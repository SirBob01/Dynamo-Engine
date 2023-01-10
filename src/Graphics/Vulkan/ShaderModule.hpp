#pragma once

#include <fstream>
#include <functional>
#include <vector>

#include <spirv_reflect.h>
#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Descriptor set binding information
     *
     */
    struct DescriptorBinding {
        unsigned set;
        vk::DescriptorSetLayoutBinding binding;
    };

    /**
     * @brief Wrapper class for a Vulkan shader module which can be plugged into
     * a Vulkan pipeline
     *
     */
    class ShaderModule {
        std::vector<char> _bytecode;
        std::reference_wrapper<Device> _device;

        std::string _filename;
        vk::ShaderStageFlagBits _stage;

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
         * @brief Construct a new ShaderModule object
         *
         * @param device   Reference to the logical device
         * @param filename Path to the shader source file
         * @param stage    Stage of the pipeline it represents
         */
        ShaderModule(Device &device,
                     const std::string filename,
                     vk::ShaderStageFlagBits stage);

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
         * @brief Get the stage of the pipeline the shader represents
         *
         * @return const vk::ShaderStageFlagBits
         */
        vk::ShaderStageFlagBits get_stage() const;

        /**
         * @brief Get the bytecode of the shader program
         *
         * @return const std::vector<char>&
         */
        const std::vector<char> &get_bytecode();

        /**
         * @brief Get the filename of the shader module
         *
         * @return const std::string&
         */
        const std::string &get_filename() const;

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
    using ShaderList = std::vector<std::reference_wrapper<ShaderModule>>;
} // namespace Dynamo::Graphics::Vulkan