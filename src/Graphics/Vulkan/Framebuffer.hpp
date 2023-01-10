#pragma once

#include <vulkan/vulkan.hpp>

#include "./Allocator.hpp"
#include "./Device.hpp"
#include "./Image.hpp"
#include "./ImageView.hpp"
#include "./RenderPass.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan Framebuffer
     *
     */
    class Framebuffer {
        vk::Framebuffer _handle;
        std::reference_wrapper<Device> _device;

      public:
        /**
         * @brief Construct a new Framebuffer object
         *
         * @param device         Reference to the device
         * @param extent         Dimensions of the framebuffer
         * @param renderpass     Reference to the renderpass
         * @param color_view     Color buffer image view
         * @param depth_view     Depth buffer image view
         * @param swapchain_view Swapchain image view
         */
        Framebuffer(Device &device,
                    vk::Extent2D &extent,
                    RenderPass &renderpass,
                    ImageView &color_view,
                    ImageView &depth_view,
                    ImageView &swapchain_view);

        /**
         * @brief Destroy the Framebuffer object
         *
         */
        ~Framebuffer();

        /**
         * @brief Get the handle to vk::Framebuffer
         *
         * @return const vk::Framebuffer&
         */
        const vk::Framebuffer &get_handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan