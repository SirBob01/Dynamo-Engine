#include "./Framebuffer.hpp"

namespace Dynamo::Graphics::Vulkan {
    Framebuffer::Framebuffer(Device &device,
                             vk::Extent2D &extent,
                             RenderPass &renderpass,
                             ImageView &color_view,
                             ImageView &depth_view,
                             ImageView &swapchain_view) :
        _device(device) {
        std::array<vk::ImageView, 3> views = {
            color_view.get_handle(),
            depth_view.get_handle(),
            swapchain_view.get_handle(),
        };
        vk::FramebufferCreateInfo fb_info;
        fb_info.renderPass = renderpass.get_handle();
        fb_info.attachmentCount = views.size();
        fb_info.pAttachments = views.data();
        fb_info.width = extent.width;
        fb_info.height = extent.height;
        fb_info.layers = 1;

        _handle = _device.get().get_handle().createFramebuffer(fb_info);
    }

    Framebuffer::~Framebuffer() {
        _device.get().get_handle().destroyFramebuffer(_handle);
    }

    const vk::Framebuffer &Framebuffer::get_handle() const { return _handle; }
} // namespace Dynamo::Graphics::Vulkan