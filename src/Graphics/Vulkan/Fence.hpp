#pragma once

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class of a Vulkan fence for CPU-GPU synchronization
     *
     * When a command is sent to a queue, a fence can be signaled to tell the
     * CPU that the GPU is currently at work
     *
     * The CPU can then wait for that fence to finish before proceeding with
     * its process
     *
     */
    class Fence {
        vk::Fence _handle;
        std::reference_wrapper<Device> _device;

      public:
        /**
         * @brief Construct a new Fence object
         *
         * @param device
         */
        Fence(Device &device);

        /**
         * @brief Destroy the Fence object
         *
         */
        ~Fence();

        /**
         * @brief Get the handle to vk::Fence
         *
         * @return const vk::Fence&
         */
        const vk::Fence &get_handle() const;

        /**
         * @brief Block the process until the fence finishes
         *
         * @param timeout
         */
        void wait(unsigned long timeout = UINT64_MAX);

        /**
         * @brief Test if the fence is signaled
         *
         * @return true
         * @return false
         */
        bool is_signaled();

        /**
         * @brief Reset the fence
         *
         */
        void reset();
    };
} // namespace Dynamo::Graphics::Vulkan