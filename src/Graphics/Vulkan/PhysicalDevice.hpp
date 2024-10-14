#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan Queue Family.
     *
     */
    struct QueueFamily {
        unsigned index = 0;
        unsigned count = 0;
        std::vector<float> priorities;
    };
    using QueueFamilyRef = std::reference_wrapper<const QueueFamily>;

    /**
     * @brief Available swapchain options.
     *
     */
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
         * @brief Create PhysicalDevice object.
         *
         * @param handle
         * @param surface
         */
        PhysicalDevice(VkPhysicalDevice handle, VkSurfaceKHR surface);
        PhysicalDevice() = default;

        /**
         * @brief Select the best available physical device.
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
         * @brief Get the unique set of queue families
         *
         * @return std::vector<QueueFamilyRef>
         */
        std::vector<QueueFamilyRef> unique_queue_families() const;

        /**
         * @brief Get the set of required extensions.
         *
         * @return std::vector<const char *>
         */
        std::vector<const char *> required_extensions() const;

        /**
         * @brief Compute the desirability "score".
         *
         * @return unsigned
         */
        unsigned score() const;
    };
} // namespace Dynamo::Graphics::Vulkan