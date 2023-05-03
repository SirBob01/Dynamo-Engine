#pragma once

#include <array>
#include <functional>
#include <map>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../../Log/Log.hpp"
#include "../../../Types.hpp"
#include "./Device.hpp"
#include "./ShaderModule.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Auxillary datatypes to group descriptor bindings from each
     * shader stage by set index.
     *
     */
    using LayoutBindings = std::vector<vk::DescriptorSetLayoutBinding>;
    using BindingGroups = std::map<u32, LayoutBindings>;

    /**
     * @brief Wrapper class for a Vulkan pipeline layout. This describes what
     * descriptor sets and push constants are used by the pipeline.
     *
     */
    class PipelineLayout {
        vk::PipelineLayout _handle;
        std::reference_wrapper<Device> _device;

        ShaderList _shaders;

        std::vector<vk::PushConstantRange> _push_constant_ranges;
        std::vector<LayoutBindings> _descriptor_set_bindings;
        std::vector<vk::DescriptorSetLayout> _descriptor_set_layouts;

      public:
        /**
         * @brief Construct a new PipelineLayout object.
         *
         * @param device  Reference to the logical device
         * @param shaders List of shader stage modules
         */
        PipelineLayout(Device &device, ShaderList &shaders);

        /**
         * @brief Destroy the PipelineLayout object.
         *
         */
        ~PipelineLayout();

        /**
         * @brief Get the handle object.
         *
         * @return const vk::PipelineLayout&
         */
        const vk::PipelineLayout &get_handle() const;

        /**
         * @brief Get the shader list.
         *
         * @return const ShaderList&
         */
        const ShaderList &get_shaders() const;

        /**
         * @brief Get the descriptor set layout list.
         *
         * @return const std::vector<vk::DescriptorSetLayout>&
         */
        const std::vector<vk::DescriptorSetLayout> &
        get_descriptor_set_layouts() const;

        /**
         * @brief Get the descriptor set bindings list.
         *
         * @return const std::vector<LayoutBindings>&
         */
        const std::vector<LayoutBindings> &get_descriptor_set_bindings() const;
    };
} // namespace Dynamo::Graphics::Vulkan