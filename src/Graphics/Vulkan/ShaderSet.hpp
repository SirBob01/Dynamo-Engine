#pragma once

#include <string>
#include <vector>

#include <spirv_reflect.h>
#include <vulkan/vulkan_core.h>

#include <Graphics/Shader.hpp>
#include <Utils/SparseSet.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Shader module instance.
     *
     */
    struct ShaderModule {
        VkShaderModule handle;
        std::vector<VkVertexInputBindingDescription> bindings;
        std::vector<VkVertexInputAttributeDescription> attributes;
    };

    /**
     * @brief Shader set.
     *
     */
    class ShaderSet {
        VkDevice _device;
        IdTracker _ids;
        SparseSet<ShaderModule> _modules;

        /**
         * @brief Compile a shader source.
         *
         * @param name
         * @param code
         * @param stage
         * @param optimized
         * @return std::vector<uint32_t>
         */
        std::vector<uint32_t>
        compile(const std::string &name, const std::string &code, VkShaderStageFlagBits stage, bool optimized);

        /**
         * @brief Extract vertex inputs from the shader source.
         *
         * @param module
         * @param reflection
         */
        void reflect_vertex_input(ShaderModule &module, SpvReflectShaderModule reflection);

      public:
        ShaderSet(VkDevice device);
        ShaderSet() = default;

        /**
         * @brief Get a shader module.
         *
         * @param shader
         * @return ShaderModule&
         */
        ShaderModule &get(Shader shader);

        /**
         * @brief Build a shader module from a descriptor.
         *
         * @param descriptor
         * @return Shader
         */
        Shader build(const ShaderDescriptor &descriptor);

        /**
         * @brief Destroy a shader module.
         *
         * @param shader
         */
        void destroy(Shader shader);

        /**
         * @brief Destroy all existing shader modules.
         *
         */
        void destroy();
    };
} // namespace Dynamo::Graphics::Vulkan