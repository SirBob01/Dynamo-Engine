#pragma once

#include <vulkan/vulkan.h>

#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/Surface.hpp>

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

    class PhysicalDevice {
        VkPhysicalDevice _handle;

        VkPhysicalDeviceProperties _properties;
        VkPhysicalDeviceMemoryProperties _memory_properties;
        VkPhysicalDeviceFeatures _features;

        std::vector<VkQueueFamilyProperties> _queue_families;
        QueueFamily _graphics_queues;
        QueueFamily _present_queues;
        QueueFamily _compute_queues;
        QueueFamily _transfer_queues;

        SwapchainOptions _swapchain_options;

        std::vector<VkExtensionProperties> _extensions;
        std::vector<const char *> _required_extensions;

        /**
         * @brief Check if the device supports all required extensions.
         *
         * @return true
         * @return false
         */
        bool supports_required_extensions() const;

      public:
        PhysicalDevice(VkPhysicalDevice handle, Surface &surface);

        VkPhysicalDevice handle() const;

        const VkPhysicalDeviceProperties &properties() const;

        const VkPhysicalDeviceMemoryProperties &memory_properties() const;

        const VkPhysicalDeviceFeatures &features() const;

        const std::vector<VkQueueFamilyProperties> &queue_families() const;

        const std::vector<VkExtensionProperties> &extensions() const;

        const std::vector<const char *> &required_extensions() const;

        const QueueFamily &graphics_queues() const;

        const QueueFamily &present_queues() const;

        const QueueFamily &transfer_queues() const;

        const QueueFamily &compute_queues() const;

        const SwapchainOptions &swapchain_options() const;

        /**
         * @brief Compute score for default device selection.
         *
         * @return int
         */
        int score() const;
    };
} // namespace Dynamo::Graphics::Vulkan