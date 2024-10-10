#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper for Vulkan shader module.
     *
     */
    struct Shader {
        VkShaderModule handle;
        VkDevice device;
        VkShaderStageFlagBits stage;
        std::string name;

        /**
         * @brief Build a shader module from source.
         *
         * @param device
         * @param name
         * @param code
         * @param stage
         * @return VkShaderModule
         */
        static Shader
        build(VkDevice device, const std::string &name, const std::string &code, VkShaderStageFlagBits stage);

        /**
         * @brief Compile a shader source.
         *
         * @param name
         * @param code
         * @param stage
         * @return std::vector<uint32_t>
         */
        static std::vector<uint32_t>
        compile(const std::string &name, const std::string &code, VkShaderStageFlagBits stage);

        /**
         * @brief Destroy the shader module.
         *
         */
        void destroy();
    };
} // namespace Dynamo::Graphics::Vulkan