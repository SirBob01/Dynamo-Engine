#pragma once

#include <array>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "../../Types.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Enumerates the different queue families
     *
     */
    enum class QueueFamily : u32 {
        /**
         * @brief Graphics commands
         *
         */
        Graphics,

        /**
         * @brief Buffer transfer commands
         *
         */
        Transfer,

        /**
         * @brief Presentation commands
         *
         */
        Present
    };

    /**
     * @brief Properties of a command queue
     *
     */
    struct QueueProperties {
        /**
         * @brief Family identifier
         *
         */
        u32 family_id = 0;

        /**
         * @brief Number of available queues
         *
         */
        u32 count = 0;
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
    class PhysicalDevice {
        vk::PhysicalDevice _handle;
        vk::SurfaceKHR _surface;

        vk::PhysicalDeviceProperties _properties;
        vk::PhysicalDeviceMemoryProperties _memory_properties;
        vk::PhysicalDeviceFeatures _features;

        SwapchainOptions _swapchain_options;

        QueueProperties _graphics_queue_properties;
        QueueProperties _present_queue_properties;
        QueueProperties _transfer_queue_properties;

        std::vector<const i8 *> _extensions;

        /**
         * @brief Check if the device contains the required queue families
         *
         * @return true
         * @return false
         */
        b8 is_complete() const;

        /**
         * @brief Check if the device supports the required extensions
         *
         * @return true
         * @return false
         */
        b8 is_supporting_extensions() const;

        /**
         * @brief Check if the device supports swapchaining
         *
         * @return true
         * @return false
         */
        b8 is_supporting_swapchain() const;

        /**
         * @brief Enumerate the available queue familes
         *
         */
        void enumerate_command_queues();

      public:
        /**
         * @brief Construct a new Physical object
         *
         * @param handle  Vulkan PhysicalDevice handle
         * @param surface Vulkan window surface
         */
        PhysicalDevice(vk::PhysicalDevice handle, vk::SurfaceKHR surface);

        /**
         * @brief Get the handle object to the vk::PhysicalDevice
         *
         * @return const vk::PhysicalDevice&
         */
        const vk::PhysicalDevice &get_handle() const;

        /**
         * @brief Get the name of the device
         *
         * @return std::string
         */
        const std::string get_name() const;

        /**
         * @brief Get the extensions list
         *
         * @return const std::vector<const i8 *>&
         */
        const std::vector<const i8 *> &get_extensions() const;

        /**
         * @brief Get the available options for the swapchain
         *
         * @return const SwapchainOptions&
         */
        const SwapchainOptions &get_swapchain_options();

        /**
         * @brief Get the properties of a queue
         *
         * @return const QueueProperties&
         */

        /**
         * @brief Get the properties of a queue
         *
         * @param family Class of commands handled by the queue
         * @return const QueueProperties&
         */
        const QueueProperties &get_queue_properties(QueueFamily family) const;

        /**
         * @brief Get the properties of a pixel format
         *
         * @param format Pixel format
         * @return vk::FormatProperties
         */
        vk::FormatProperties get_format_properties(vk::Format format) const;

        /**
         * @brief Get the memory properties of the hardware
         *
         * @return const vk::PhysicalDeviceMemoryProperties
         */
        const vk::PhysicalDeviceMemoryProperties &get_memory_properties() const;

        /**
         * @brief Get the limits of the device
         *
         * @return const vk::PhysicalDeviceLimits&
         */
        const vk::PhysicalDeviceLimits &get_limits() const;

        /**
         * @brief Get the anti-aliasing sample count supported by the device
         *
         * @return vk::SampleCountFlagBits
         */
        vk::SampleCountFlagBits get_msaa_samples() const;

        /**
         * @brief Get the supported pixel format for the depth buffer
         *
         * @return vk::Format
         */
        vk::Format get_depth_format() const;

        /**
         * @brief Calculate heuristic for selecting the best device
         *
         * @return i32
         */
        i32 calculate_score() const;
    };
} // namespace Dynamo::Graphics::Vulkan