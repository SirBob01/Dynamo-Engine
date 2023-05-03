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
     * dependencies.
     *
     * TODO: Does it make sense for this to be a class? RenderPass is a very
     * static object with a default state. It might be better to write
     * create_*_renderpass() functions instead if we plan to make custom
     * passes / subpasses.
     *
     */
    class RenderPass {
        vk::RenderPass _handle;
        std::reference_wrapper<Device> _device;
        std::reference_wrapper<Swapchain> _swapchain;

        /**
         * @brief Create a color attachment
         *
         * @return vk::AttachmentDescription
         */
        vk::AttachmentDescription create_color_attachment();

        /**
         * @brief Create a depth attachment
         *
         * @return vk::AttachmentDescription
         */
        vk::AttachmentDescription create_depth_attachment();

        /**
         * @brief Create a resolve attachment for the multisampling pass
         *
         * @return vk::AttachmentDescription
         */
        vk::AttachmentDescription create_resolve_attachment();

        /**
         * @brief Create the list of all subpasses
         *
         * @param color_ref   Color attachment reference
         * @param depth_ref   Depth attachment reference
         * @param resolve_ref Resolve attachment reference
         * @return std::vector<vk::SubpassDescription>
         */
        std::vector<vk::SubpassDescription>
        create_subpasses(vk::AttachmentReference &color_ref,
                         vk::AttachmentReference &depth_ref,
                         vk::AttachmentReference &resolve_ref);

        /**
         * @brief Create a subpass dependencies, which defines the connectivity
         * of the subpass graph
         *
         * @return std::vector<vk::SubpassDependency>
         */
        std::vector<vk::SubpassDependency> create_subpass_dependencies();

      public:
        /**
         * @brief Construct a new RenderPass object
         *
         * @param device    Reference to the logical device
         * @param swapchain Reference to the swapchain
         */
        RenderPass(Device &device, Swapchain &swapchain);

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
    };
} // namespace Dynamo::Graphics::Vulkan