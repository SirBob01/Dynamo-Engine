#pragma once

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class of a Vulkan semaphore for queue synchronization
     *
     * Vulkan does not guarantee that commands submitted one after another on
     * different queues will finish in that order, since those commands can
     * run in parallel
     *
     * A semaphore is used to ensure that the commands in one queue waits for
     * the commands in another queue to finish
     *
     */
    class Semaphore {
        vk::Semaphore _handle;
        std::reference_wrapper<Device> _device;

      public:
        /**
         * @brief Construct a new Semaphore object
         *
         * @param device
         */
        Semaphore(Device &device);

        /**
         * @brief Destroy the Semaphore object
         *
         */
        ~Semaphore();

        /**
         * @brief Get the handle to vk::Semaphore
         *
         * @return const vk::Semaphore&
         */
        const vk::Semaphore &get_handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan