#pragma once

#include <optional>

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/ImageView.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper object for a Vulkan swapchain and its properties.
     *
     */
    struct Swapchain {
        VkSwapchainKHR handle;

        VkExtent2D extent;
        VkSurfaceFormatKHR surface_format;
        VkPresentModeKHR present_mode;

        std::vector<VkImage> images;
    };

    /**
     * @brief Build a Vulkan swapchain.
     *
     * @param device
     * @param physical
     * @param display
     * @param previous
     * @return Swapchain
     */
    Swapchain Swapchain_build(VkDevice device,
                              const PhysicalDevice &physical,
                              const Display &display,
                              std::optional<VkSwapchainKHR> previous = {});
} // namespace Dynamo::Graphics::Vulkan