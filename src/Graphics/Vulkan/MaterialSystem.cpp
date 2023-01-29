#include "./MaterialSystem.hpp"

namespace Dynamo::Graphics::Vulkan {
    MaterialSystem::MaterialSystem(Device &device,
                                   Swapchain &swapchain,
                                   DescriptorPool &descriptor_pool,
                                   Buffer &uniform_buffer) :
        _device(device),
        _swapchain(swapchain), _descriptor_pool(descriptor_pool),
        _uniform_buffer(uniform_buffer),
        _renderpass(create_base_3d_pass(_device, _swapchain)), _cache(device) {}

    void MaterialSystem::reset(Swapchain &swapchain) {
        _renderpass = create_base_3d_pass(_device, _swapchain);
    }

    std::unique_ptr<Dynamo::Graphics::Material>
    MaterialSystem::build(Dynamo::Graphics::ShaderList &shaders,
                          PolygonMode polygon_mode) {
        // Get the Vulkan polygon modes
        vk::PolygonMode vk_polygon_mode;
        switch (polygon_mode) {
        case PolygonMode::Fill:
            vk_polygon_mode = vk::PolygonMode::eFill;
            break;
        case PolygonMode::Line:
            vk_polygon_mode = vk::PolygonMode::eLine;
            break;
        case PolygonMode::Point:
        default:
            vk_polygon_mode = vk::PolygonMode::ePoint;
            break;
        }

        // Downcast the shader modules
        ShaderList vk_shaders;
        for (Dynamo::Graphics::Shader &shader : shaders) {
            vk_shaders.push_back(*dynamic_cast<Shader *>(&shader));
        }

        // Create the material
        return std::make_unique<Material>(_device,
                                          _descriptor_pool,
                                          _swapchain,
                                          _renderpass,
                                          _cache,
                                          _uniform_buffer,
                                          vk_shaders,
                                          vk_polygon_mode);
    }
} // namespace Dynamo::Graphics::Vulkan