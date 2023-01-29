#pragma once

#include <fstream>
#include <functional>
#include <vector>

#include <spirv_reflect.h>
#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "../../Types.hpp"
#include "../Shader.hpp"
#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Enumerates the binding types
     *
     */
    enum class BindingType {
        /**
         * @brief Raw data binding
         *
         */
        Buffer,

        /**
         * @brief Texture binding
         *
         */
        Texture
    };

    /**
     * @brief Descriptor set binding information
     *
     */
    struct DescriptorBinding {
        /**
         * @brief Name of the binding variable
         *
         */
        std::string name;

        /**
         * @brief Type of binding
         *
         */
        BindingType type;

        /**
         * @brief API Binding struct
         *
         */
        vk::DescriptorSetLayoutBinding binding;

        /**
         * @brief Set index
         *
         */
        u32 set;

        /**
         * @brief Size in bytes
         *
         */
        u32 size;
    };

    /**
     * @brief Convert the API agnostic shader stage enum to a Vulkan shader
     * stage enum
     *
     * @param stage
     * @return vk::ShaderStageFlagBits
     */
    inline vk::ShaderStageFlagBits convert_shader_stage(ShaderStage stage) {
        switch (stage) {
        case ShaderStage::Vertex:
            return vk::ShaderStageFlagBits::eVertex;
        case ShaderStage::Geometry:
            return vk::ShaderStageFlagBits::eGeometry;
        case ShaderStage::Fragment:
            return vk::ShaderStageFlagBits::eFragment;
        case ShaderStage::Compute:
        default:
            return vk::ShaderStageFlagBits::eCompute;
        }
    }

    /**
     * @brief Wrapper class for a Vulkan shader module which can be plugged into
     * a Vulkan pipeline
     *
     */
    class Shader : public Dynamo::Graphics::Shader {
        std::vector<char> _bytecode;
        std::reference_wrapper<Device> _device;

        std::string _filename;
        ShaderStage _stage;

        std::vector<DescriptorBinding> _descriptor_bindings;
        std::vector<vk::PushConstantRange> _push_constant_ranges;

        /**
         * @brief Parse the descriptor set layouts
         *
         */
        void parse_descriptor_layouts(SpvReflectShaderModule &module);

        /**
         * @brief Parse the push constants from the bytecode
         *
         */
        void parse_push_constants(SpvReflectShaderModule &module);

      public:
        /**
         * @brief Construct a new Shader object
         *
         * @param device   Reference to the logical device
         * @param filename Path to the shader source file
         */
        Shader(Device &device, const std::string filename);

        /**
         * @brief Destroy the Shader object
         *
         */
        virtual ~Shader() = default;

        /**
         * @brief Get the handle to vk::UniqueShaderModule
         *
         * This is a temporary object that should be destroyed as soon as
         * pipeline creation ceases
         *
         * @return const vk::UniqueShaderModule&
         */
        vk::UniqueShaderModule get_handle();

        /**
         * @brief Get the filename of the shader module
         *
         * @return const std::string&
         */
        const std::string &get_filename() const override;

        /**
         * @brief Get the bytecode of the shader program
         *
         * @return const std::vector<char>&
         */
        const std::vector<char> &get_bytecode() const override;

        /**
         * @brief Get the stage of the pipeline the shader represents
         *
         * @return ShaderStage
         */
        ShaderStage get_stage() const override;

        /**
         * @brief Get the push constant ranges
         *
         * @return const std::vector<vk::PushConstantRange>&
         */
        const std::vector<vk::PushConstantRange> &
        get_push_constant_ranges() const;

        /**
         * @brief Get the descriptor bindings accessed from this shader
         *
         * @return const std::vector<DescriptorBinding>&
         */
        const std::vector<DescriptorBinding> &get_descriptor_bindings() const;
    };

    /**
     * @brief List of shader stage modules
     *
     */
    using ShaderList = std::vector<std::reference_wrapper<Shader>>;
} // namespace Dynamo::Graphics::Vulkan