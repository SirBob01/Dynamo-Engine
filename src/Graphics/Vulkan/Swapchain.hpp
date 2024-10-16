#pragma once

#include <optional>

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper object for a Vulkan swapchain and its properties.
     *
     */
    struct Swapchain {
        VkDevice device;
        VkSwapchainKHR handle;

        VkExtent2D extent;
        VkSurfaceFormatKHR surface_format;
        VkPresentModeKHR present_mode;

        std::vector<VkImage> images;
        std::vector<VkImageView> views;

        /**
         * @brief Create a Vulkan swapchain.
         *
         * @param device
         * @param physical
         * @param display
         * @param previous
         * @return Swapchain
         */
        Swapchain(VkDevice device,
                  const PhysicalDevice &physical,
                  const Display &display,
                  std::optional<Swapchain> previous = {});
        Swapchain() = default;

        /**
         * @brief Destroy the swapchain and its resources.
         *
         */
        void destroy();
    };
} // namespace Dynamo::Graphics::Vulkan