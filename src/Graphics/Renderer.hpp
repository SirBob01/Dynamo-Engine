#pragma once

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/CommandPool.hpp>
#include <Graphics/Vulkan/Debugger.hpp>
#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/ImageView.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Graphics/Vulkan/Shader.hpp>
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

        Shader _vertex;
        Shader _fragment;
        VkPipelineLayout _layout;
        VkRenderPass _renderpass;
        VkPipeline _pipeline;
        std::vector<VkFramebuffer> _framebuffers;

        VkCommandPool _command_pool;
        VkCommandBuffer _command_buffer;

        VkFence _f_frame_ready;
        VkSemaphore _s_render_start;
        VkSemaphore _s_render_done;

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
         * @brief Refresh the renderer.
         *
         */
        void refresh();
    };
} // namespace Dynamo::Graphics::Vulkan