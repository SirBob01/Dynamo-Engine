#include <Graphics/Vulkan/ShaderCache.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <Utils/Log.hpp>
#include <shaderc/shaderc.hpp>

namespace Dynamo::Graphics::Vulkan {
    ShaderCache::ShaderCache(VkDevice device) : _device(device) {}

    std::vector<uint32_t>
    ShaderCache::compile(const std::string &name, const std::string &code, VkShaderStageFlagBits stage) {
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

    VkShaderModule ShaderCache::build(const ShaderKey &key) {
        auto shader_it = _cache.find(key);
        if (shader_it != _cache.end()) {
            return shader_it->second;
        }

        std::vector<uint32_t> bytecode = compile(VkShaderStageFlagBits_string(key.stage), key.code, key.stage);
        VkShaderModule shader = VkShaderModule_create(_device, bytecode);

        _cache.emplace(key, shader);

        return shader;
    }

    void ShaderCache::destroy() {
        for (const auto &[key, shader] : _cache) {
            vkDestroyShaderModule(_device, shader, nullptr);
        }
        _cache.clear();
    }
} // namespace Dynamo::Graphics::Vulkan