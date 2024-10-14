#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan_core.h>

#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Shader cache to minimize expensive compiles.
     *
     */
    class ShaderCache {
        VkDevice _device;
        std::unordered_map<ShaderKey, VkShaderModule> _cache;

        /**
         * @brief Compile a shader source.
         *
         * @param name
         * @param code
         * @param stage
         * @return std::vector<uint32_t>
         */
        std::vector<uint32_t> compile(const std::string &name, const std::string &code, VkShaderStageFlagBits stage);

      public:
        ShaderCache(VkDevice device);
        ShaderCache() = default;

        /**
         * @brief Build a shader module from source.
         *
         * @param key
         * @return VkShaderModule
         */
        VkShaderModule build(const ShaderKey &key);

        /**
         * @brief Destroy the shader cache.
         *
         */
        void destroy();
    };
} // namespace Dynamo::Graphics::Vulkan