#pragma once

#include <Math/Color.hpp>
#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/Command.hpp>
#include <Graphics/Vulkan/Debugger.hpp>
#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/ImageView.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Graphics/Vulkan/Shader.hpp>
#include <Graphics/Vulkan/Swapchain.hpp>
#include <Graphics/Vulkan/Synchronize.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan-powered 3D Renderer.
     *
     */
    class Renderer {
        const Display &_display;

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugger;
        VkSurfaceKHR _surface;

        PhysicalDevice _physical;
        VkDevice _device;

        Swapchain _swapchain;
        VkClearValue _clear;

        // This should be cached somewhere {
        Shader _vertex;
        Shader _fragment;
        VkPipelineLayout _layout;
        VkRenderPass _renderpass;
        VkPipeline _pipeline;
        // }

        VkCommandPool _graphics_pool;

        std::vector<VkImageView> _views;
        std::vector<VkFramebuffer> _framebuffers;

        struct FrameContext {
            VkFence sync_fence;
            VkSemaphore sync_render_start;
            VkSemaphore sync_render_done;
            VkCommandBuffer command_buffer;
        };
        std::array<FrameContext, 3> _frame_context;
        unsigned _current_frame;

        /**
         * @brief Rebuild the swapchain.
         *
         */
        void rebuild_swapchain();

      public:
        /**
         * @brief Initialize the renderer.
         *
         * @param display
         */
        Renderer(const Display &display);

        /**
         * @brief Cleanup and destroy the renderer.
         *
         */
        ~Renderer();

        /**
         * @brief Set the clear color.
         *
         * @param color
         */
        void set_clear(Color color);

        /**
         * @brief Refresh the renderer.
         *
         */
        void refresh();
    };
} // namespace Dynamo::Graphics::Vulkan