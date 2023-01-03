#pragma once

#include <set>

#include <vulkan/vulkan.hpp>

#include "./PhysicalDevice.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan logical device
     *
     */
    class Device {
        vk::UniqueDevice _handle;
        PhysicalDevice _physical;

      public:
        /**
         * @brief Construct a new Device object
         *
         * @param physical
         */
        Device(PhysicalDevice &physical);

        /**
         * @brief Destroy the Device object
         *
         */
        ~Device();

        /**
         * @brief Get the handle to vk::Device
         *
         * @return vk::Device&
         */
        vk::Device &get_handle();

        /**
         * @brief Get the physical device
         *
         * @return PhysicalDevice&
         */
        PhysicalDevice &get_physical();

        /**
         * @brief Get the graphics queue
         *
         * @param family
         * @param index
         * @return vk::Queue
         */
        vk::Queue get_queue(QueueFamily family, unsigned index = 0);

        /**
         * @brief Wait for all queue operations to finish
         *
         */
        void wait_idle();
    };
} // namespace Dynamo::Graphics::Vulkan