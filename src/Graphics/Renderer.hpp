#pragma once

#include <Math/Color.hpp>
#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/Buffer.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Graphics/Vulkan/PipelineCache.hpp>
#include <Graphics/Vulkan/ShaderCache.hpp>
#include <Graphics/Vulkan/Swapchain.hpp>

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

        ShaderCache _shader_cache;
        PipelineCache _pipeline_cache;

        VkCommandPool _graphics_pool;
        VkCommandPool _transfer_pool;

        Buffer _vertex_buffer;
        Buffer _index_buffer;
        Buffer _staging_buffer;

        // These should be cached somewhere {
        VkPipelineLayout _layout;
        PipelinePass _pipeline_pass;
        // }

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
         * @param root_asset_directory
         */
        Renderer(const Display &display, const std::string &root_asset_directory);

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