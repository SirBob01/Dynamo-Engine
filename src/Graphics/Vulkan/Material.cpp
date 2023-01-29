#include "./Material.hpp"

namespace Dynamo::Graphics::Vulkan {
    Material::Material(Device &device,
                       DescriptorPool &descriptor_pool,
                       Swapchain &swapchain,
                       RenderPass &renderpass,
                       PipelineCache &cache,
                       Buffer &uniform_buffer,
                       ShaderList &shaders,
                       vk::PolygonMode polygon_mode) :
        _device(device),
        _renderpass(renderpass), _layout(device, shaders),
        _pipeline(device,
                  renderpass,
                  swapchain,
                  _layout,
                  cache,
                  vk::PrimitiveTopology::eTriangleList,
                  polygon_mode) {
        // Allocate the descriptor sets for this material
        _descriptor_set_groups = descriptor_pool.allocate(_layout, swapchain);

        // Map each descriptor set to their respective buffers and textures
        for (std::reference_wrapper<Shader> shader : shaders) {
            shader.get().get_descriptor_bindings();
        }
        for (std::vector<vk::DescriptorSet> &sets : _descriptor_set_groups) {
            for (vk::DescriptorSet set : sets) {
            }
        }
    }

    const Pipeline &Material::get_pipeline() const { return _pipeline; }

    const PipelineLayout &Material::get_layout() const { return _layout; }

    const RenderPass &Material::get_renderpass() const {
        return _renderpass.get();
    }

    const std::vector<DescriptorSetGroup> &
    Material::get_descriptor_set_groups() const {
        return _descriptor_set_groups;
    }

    void Material::set(const std::string field, u8 *src, u32 size) {
        // TODO: Copy data into the buffer
    }

    u32 Material::hash() const {
        // return &_pipeline.get_handle()
    }

    bool Material::operator==(const Material &rhs) const {
        // TODO
    }
} // namespace Dynamo::Graphics::Vulkan