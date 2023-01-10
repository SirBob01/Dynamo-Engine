#include "./PipelineLayout.hpp"

namespace Dynamo::Graphics::Vulkan {
    PipelineLayout::PipelineLayout(Device &device, ShaderList &shaders) :
        _device(device), _shaders(shaders) {
        // Extract reflection information from the shaders to construct
        // descriptor layouts and push constant ranges
        BindingGroups binding_groups;
        for (std::reference_wrapper<ShaderModule> shader : _shaders) {
            for (const vk::PushConstantRange &range :
                 shader.get().get_push_constant_ranges()) {
                _push_constant_ranges.push_back(range);
            }
            for (const DescriptorBinding &data :
                 shader.get().get_descriptor_bindings()) {
                LayoutBindings &bindings = binding_groups[data.set];

                // Find duplicate binding in the set (i.e., from another shader
                // stage) and merge their flags
                bool found = false;
                for (vk::DescriptorSetLayoutBinding &rhs : bindings) {
                    if (rhs.binding == data.binding.binding) {
                        rhs.stageFlags |= data.binding.stageFlags;
                        found = true;
                    }
                }
                if (!found) {
                    bindings.push_back(data.binding);
                }
            }
        }

        // Group bindings with similar set indexes into descriptor layouts
        for (auto [set, bindings] : binding_groups) {
            vk::DescriptorSetLayoutCreateInfo layout_info;
            layout_info.bindingCount = bindings.size();
            layout_info.pBindings = bindings.data();

            vk::DescriptorSetLayout layout =
                _device.get().get_handle().createDescriptorSetLayout(
                    layout_info);
            _descriptor_set_layouts.push_back(layout);
            _descriptor_set_bindings.push_back(bindings);
        }

        // Create the pipeline layout
        vk::PipelineLayoutCreateInfo layout_info;
        layout_info.setLayoutCount = _descriptor_set_layouts.size();
        layout_info.pSetLayouts = _descriptor_set_layouts.data();
        layout_info.pushConstantRangeCount = _push_constant_ranges.size();
        layout_info.pPushConstantRanges = _push_constant_ranges.data();

        _handle = _device.get().get_handle().createPipelineLayout(layout_info);
    }

    PipelineLayout::~PipelineLayout() {
        // Destroy the pipeline layout
        _device.get().get_handle().destroyPipelineLayout(_handle);

        // Destroy the descriptor set layouts
        for (const vk::DescriptorSetLayout &layout : _descriptor_set_layouts) {
            _device.get().get_handle().destroyDescriptorSetLayout(layout);
        }
    }

    const vk::PipelineLayout &PipelineLayout::get_handle() const {
        return _handle;
    }

    const ShaderList &PipelineLayout::get_shaders() const { return _shaders; }

    const std::vector<vk::DescriptorSetLayout> &
    PipelineLayout::get_descriptor_set_layouts() const {
        return _descriptor_set_layouts;
    }

    const std::vector<LayoutBindings> &
    PipelineLayout::get_descriptor_set_bindings() const {
        return _descriptor_set_bindings;
    }
} // namespace Dynamo::Graphics::Vulkan