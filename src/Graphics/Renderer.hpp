#pragma once

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/Debugger.hpp>
#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/Vulkan/ImageView.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
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