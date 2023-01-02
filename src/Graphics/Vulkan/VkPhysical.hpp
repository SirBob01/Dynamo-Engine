#pragma once

#include <set>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Dynamo::Graphics {
    /**
     * @brief Properties of a command queue
     *
     */
    struct QueueProperties {
        /**
         * @brief Family identifier
         *
         */
        unsigned family_id = 0;

        /**
         * @brief Number of available queues
         *
         */
        unsigned count = 0;
    };

    /**
     * @brief Available options for creating a swapchain
     *
     */
    struct SwapchainOptions {
        /**
         * @brief Surface capabilities
         *
         */
        vk::SurfaceCapabilitiesKHR capabilities;

        /**
         * @brief Pixel formats and color spaces
         *
         */
        std::vector<vk::SurfaceFormatKHR> formats;

        /**
         * @brief Presentation modes
         *
         */
        std::vector<vk::PresentModeKHR> present_modes;
    };

    /**
     * @brief Wrapper class for a Vulkan physical device
     *
     */
    class VkPhysical {
        std::reference_wrapper<vk::PhysicalDevice> _handle;
        std::reference_wrapper<vk::SurfaceKHR> _surface;

        /**
         * @brief Device properties
         *
         */
        vk::PhysicalDeviceProperties _properties;
        vk::PhysicalDeviceMemoryProperties _memory_properties;
        vk::PhysicalDeviceFeatures _features;

        /**
         * @brief Available swapchain options
         *
         */
        SwapchainOptions _swapchain_options;

        /**
         * @brief Queue for graphics commands
         *
         */
        QueueProperties _graphics_queue_properties;

        /**
         * @brief Queue for presentation commands
         *
         */
        QueueProperties _present_queue_properties;

        /**
         * @brief Queue for buffer transfer commands
         *
         */
        QueueProperties _transfer_queue_properties;

        /**
         * @brief Required extensions
         *
         */
        std::vector<const char *> _extensions;

        /**
         * @brief Check if the device contains the required queue families
         *
         * @return true
         * @return false
         */
        bool is_complete() const;

        /**
         * @brief Check if the device supports the required extensions
         *
         * @return true
         * @return false
         */
        bool is_supporting_extensions() const;

        /**
         * @brief Check if the device supports swapchaining
         *
         * @return true
         * @return false
         */
        bool is_supporting_swapchain() const;

        /**
         * @brief Enumerate the available queue familes
         *
         */
        void enumerate_command_queues();

      public:
        /**
         * @brief Construct a new VkPhysical object
         *
         * @param handle
         * @param surface
         */
        VkPhysical(vk::PhysicalDevice &handle, vk::SurfaceKHR &surface);

        /**
         * @brief Get the handle object to the vk::PhysicalDevice
         *
         * @return vk::PhysicalDevice&
         */
        vk::PhysicalDevice &get_handle();

        /**
         * @brief Get the name of the device
         *
         * @return std::string
         */
        const std::string get_name() const;

        /**
         * @brief Get the extensions list
         *
         * @return const std::vector<const *char>&
         */
        const std::vector<const char *> &get_extensions() const;

        /**
         * @brief Get the available options for the swapchain
         *
         * @return const SwapchainOptions&
         */
        const SwapchainOptions &get_swapchain_options();

        /**
         * @brief Get the graphics queue properties
         *
         * @return const QueueProperties&
         */
        const QueueProperties &get_graphics_queue_properties() const;

        /**
         * @brief Get the presentation queue properties
         *
         * @return const QueueProperties&
         */
        const QueueProperties &get_present_queue_properties() const;

        /**
         * @brief Get the transfer queue properties
         *
         * @return const QueueProperties&
         */
        const QueueProperties &get_transfer_queue_properties() const;

        /**
         * @brief Calculate heuristic for selecting the best device
         *
         * @return int
         */
        int calculate_score() const;
    };
} // namespace Dynamo::Graphics