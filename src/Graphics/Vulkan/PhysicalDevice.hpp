#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    struct QueueFamily {
        unsigned index = 0;
        unsigned count = 0;
        float priority = 0;
    };

    struct SwapchainOptions {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> present_modes;
    };

    /**
     * @brief Wrapper object for a Vulkan physical device and its properties.
     *
     */
    struct PhysicalDevice {
        VkPhysicalDevice handle;
        VkSurfaceKHR surface;

        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceMemoryProperties memory;
        VkPhysicalDeviceFeatures features;

        QueueFamily graphics_queues;
        QueueFamily present_queues;
        QueueFamily compute_queues;
        QueueFamily transfer_queues;

        /**
         * @brief Build a Vulkan physical device.
         *
         * @param handle
         * @param surface
         * @return PhysicalDevice
         */
        static PhysicalDevice build(VkPhysicalDevice handle,
                                    VkSurfaceKHR surface);

        /**
         * @brief Select a suitable physical device based on its score.
         *
         * @param instance
         * @param surface
         * @return PhysicalDevice
         */
        static PhysicalDevice select(VkInstance instance, VkSurfaceKHR surface);

        /**
         * @brief Get the available swapchain configuration options.
         *
         * @return SwapchainOptions
         */
        SwapchainOptions get_swapchain_options() const;

        /**
         * @brief Compute the desirability "score".
         *
         * @return unsigned
         */
        unsigned score() const;
    };
} // namespace Dynamo::Graphics::Vulkan