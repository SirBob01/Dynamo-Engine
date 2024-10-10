#include <Graphics/Vulkan/Shader.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <Utils/Log.hpp>
#include <shaderc/shaderc.hpp>

namespace Dynamo::Graphics::Vulkan {
    Shader
    Shader_build(VkDevice device, const std::string &name, const std::string &code, VkShaderStageFlagBits stage) {
        std::vector<uint32_t> bytecode = Shader_compile(name, code, stage);

        Shader shader;
        shader.device = device;
        shader.stage = stage;

        VkShaderModuleCreateInfo shader_info = {};
        shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_info.pCode = bytecode.data();
        shader_info.codeSize = bytecode.size() * sizeof(uint32_t);

        VkResult_log("Create Shader Module", vkCreateShaderModule(device, &shader_info, nullptr, &shader.handle));
        return shader;
    }

    std::vector<uint32_t>
    Shader_compile(const std::string &name, const std::string &code, VkShaderStageFlagBits stage) {
        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetOptimizationLevel(shaderc_optimization_level_size);

        shaderc_shader_kind kind;
        switch (stage) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            kind = shaderc_vertex_shader;
            break;
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            kind = shaderc_fragment_shader;
            break;
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            kind = shaderc_geometry_shader;
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
} // namespace Dynamo::Graphics::Vulkan