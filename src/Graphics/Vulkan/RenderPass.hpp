#pragma once

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"
#include "./Image.hpp"
#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan render pass that defines the
     * relationships between different attachments (color, depth, and stencil
     * buffers), how these attachments are loaded and stored, and their
     * dependencies
     *
     */
    class RenderPass {
        vk::RenderPass _handle;
        std::reference_wrapper<Device> _device;
        std::reference_wrapper<Swapchain> _swapchain;

        std::vector<vk::Framebuffer> _framebuffers;

      public:
        /**
         * @brief Construct a new RenderPass object
         *
         * @param device    Reference to the logical device
         * @param swapchain Reference to the swapchain
         * @param config    Setup configuaration
         */
        RenderPass(Device &device,
                   Swapchain &swapchain,
                   vk::RenderPassCreateInfo config);

        /**
         * @brief Destroy the RenderPass object
         *
         */
        ~RenderPass();

        /**
         * @brief Get the handle to vk::RenderPass
         *
         * @return const vk::RenderPass&
         */
        const vk::RenderPass &get_handle() const;

        /**
         * @brief Get the framebuffer targets
         *
         * @return const std::vector<vk::Framebuffer>&
         */
        const std::vector<vk::Framebuffer> &get_framebuffers() const;
    };

    /**
     * @brief Create a base renderpass for 2D rendering
     *
     * @param device    Reference to the logical device
     * @param swapchain Reference to the swapchain
     * @return RenderPass
     */
    RenderPass create_base_2d_pass(Device &device, Swapchain &swapchain);

    /**
     * @brief Create a layer renderpass for 2D rendering
     *
     * @param device    Reference to the logical device
     * @param swapchain Reference to the swapchain
     * @return RenderPass
     */
    RenderPass create_layer_2d_pass(Device &device, Swapchain &swapchain);

    /**
     * @brief Create a base renderpass for 3D rendering
     *
     * @param device    Reference to the logical device
     * @param swapchain Reference to the swapchain
     * @return RenderPass
     */
    RenderPass create_base_3d_pass(Device &device, Swapchain &swapchain);

    /**
     * @brief Create a layer renderpass for 3D rendering
     *
     * @param device    Reference to the logical device
     * @param swapchain Reference to the swapchain
     * @return RenderPass
     */
    RenderPass create_layer_3d_pass(Device &device, Swapchain &swapchain);
} // namespace Dynamo::Graphics::Vulkan