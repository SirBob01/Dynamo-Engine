#include <Graphics/Vulkan/ShaderSet.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <Utils/Log.hpp>
#include <shaderc/shaderc.hpp>

namespace Dynamo::Graphics::Vulkan {
    constexpr char INSTANCE_VAR_PREFIX[] = "instance";

    ShaderSet::ShaderSet(VkDevice device) : _device(device) {}

    std::vector<uint32_t>
    ShaderSet::compile(const std::string &name, const std::string &code, VkShaderStageFlagBits stage, bool optimized) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        if (optimized) {
            options.SetOptimizationLevel(shaderc_optimization_level_performance);
        }

        shaderc_shader_kind kind;
        switch (stage) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            kind = shaderc_vertex_shader;
            break;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            kind = shaderc_fragment_shader;
            break;
        case VK_SHADER_STAGE_COMPUTE_BIT:
            kind = shaderc_compute_shader;
            break;
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            kind = shaderc_tess_control_shader;
            break;
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            kind = shaderc_tess_evaluation_shader;
            break;
        default:
            Log::error("Unsupported shader type.");
            return {};
        }

        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(code, kind, name.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            Log::error("Vulkan Shader Compile Error:\n{}", module.GetErrorMessage());
            return {};
        }

        return {module.cbegin(), module.cend()};
    }

    void ShaderSet::reflect_vertex_input(ShaderModule &module, SpvReflectShaderModule reflection) {
        unsigned count = 0;
        SpvReflectResult result = spvReflectEnumerateInputVariables(&reflection, &count, NULL);
        DYN_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        std::vector<SpvReflectInterfaceVariable *> shader_inputs(count);
        result = spvReflectEnumerateInputVariables(&reflection, &count, shader_inputs.data());
        DYN_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        for (unsigned i = 0; i < count; i++) {
            SpvReflectInterfaceVariable &input_var = *shader_inputs[i];
            VkFormat format = static_cast<VkFormat>(input_var.format);
            if (input_var.decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) {
                continue;
            }

            // Variables with prefix are per-instance
            bool instanced = std::string(input_var.name).rfind(INSTANCE_VAR_PREFIX) == 0;

            VkVertexInputBindingDescription binding = {};
            binding.binding = input_var.location;
            binding.inputRate = instanced ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
            binding.stride = VkFormat_size(format);
            module.bindings.push_back(binding);

            VkVertexInputAttributeDescription attribute = {};
            attribute.location = input_var.location;
            attribute.binding = input_var.location;
            attribute.format = format;
            attribute.offset = 0;
            module.attributes.push_back(attribute);

            Log::info("Binding (binding: {}, stride: {}, rate: {})",
                      binding.binding,
                      binding.stride,
                      instanced ? "per-instance" : "per-vertex");
            Log::info("Attribute (location: {}, binding: {})", attribute.location, attribute.binding);
        }
    }

    Shader ShaderSet::build(const ShaderDescriptor &descriptor) {
        // Determine shader stage
        VkShaderStageFlagBits stage;
        switch (descriptor.stage) {
        case ShaderStage::Vertex:
            stage = VK_SHADER_STAGE_VERTEX_BIT;
            break;
        case ShaderStage::Fragment:
            stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            break;
        case ShaderStage::Compute:
            stage = VK_SHADER_STAGE_COMPUTE_BIT;
            break;
        case ShaderStage::TesselationControl:
            stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            break;
        case ShaderStage::TesselationEvaluation:
            stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            break;
        }

        // Compile the shader code (optimized and non-optimized for reflection)
        std::vector<uint32_t> bytecode = compile(descriptor.name, descriptor.code, stage, false);
        std::vector<uint32_t> bytecode_optimized = compile(descriptor.name, descriptor.code, stage, true);

        // Create the shader module
        ShaderModule module;
        module.handle = VkShaderModule_create(_device, bytecode_optimized);

        // Reflection
        SpvReflectShaderModule reflection;
        unsigned bytecode_length = bytecode.size() * sizeof(uint32_t);
        SpvReflectResult result = spvReflectCreateShaderModule(bytecode_length, bytecode.data(), &reflection);
        DYN_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        Log::info("Shader {} reflection:", descriptor.name);
        reflect_vertex_input(module, reflection);
        spvReflectDestroyShaderModule(&reflection);
        Log::info("");

        // Register the new shader
        Shader shader = _ids.generate();
        _modules.insert(shader, module);
        return shader;
    }

    ShaderModule &ShaderSet::get(Shader shader) { return _modules.get(shader); }

    void ShaderSet::destroy(Shader shader) {
        ShaderModule &module = _modules.get(shader);
        vkDestroyShaderModule(_device, module.handle, nullptr);
        _modules.remove(shader);
        _ids.discard(shader);
    }

    void ShaderSet::destroy() {
        for (ShaderModule &module : _modules) {
            vkDestroyShaderModule(_device, module.handle, nullptr);
        }
        _modules.clear();
        _ids.clear();
    }
} // namespace Dynamo::Graphics::Vulkan