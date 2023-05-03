#pragma once

#include <set>

#include <vulkan/vulkan.hpp>

#include "../../../Types.hpp"
#include "./PhysicalDevice.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan logical device. This is the interface
     * from which most Vulkan operations are performed to interact with the
     * physical device.
     *
     */
    class Device {
        vk::Device _handle;
        PhysicalDevice _physical;

      public:
        /**
         * @brief Construct a new Device object.
         *
         * @param physical Physical device
         */
        Device(PhysicalDevice &physical);

        /**
         * @brief Destroy the Device object.
         *
         */
        ~Device();

        /**
         * @brief Get the handle to vk::Device.
         *
         * @return const vk::Device&
         */
        const vk::Device &get_handle() const;

        /**
         * @brief Get the physical device.
         *
         * @return PhysicalDevice&
         */
        PhysicalDevice &get_physical();

        /**
         * @brief Get the graphics queue.
         *
         * @param family Class of commands handled by the queue
         * @param index  Queue index
         * @return vk::Queue
         */
        vk::Queue get_queue(QueueFamily family, u32 index = 0);

        /**
         * @brief Wait for all queue operations to finish.
         *
         */
        void wait();
    };
} // namespace Dynamo::Graphics::Vulkan