#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"
#include "./PhysicalDevice.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan command pool
     *
     */
    class CommandAllocator {
        vk::CommandPool _handle;
        std::reference_wrapper<Device> _device;
        QueueFamily _family;

      public:
        /**
         * @brief Construct a new CommandAllocator object
         *
         * @param device Reference to the logical device
         * @param family Queue family
         */
        CommandAllocator(Device &device, QueueFamily family);

        /**
         * @brief Destroy the CommandAllocator object
         *
         */
        ~CommandAllocator();

        /**
         * @brief Get the handle to vk::CommandPool
         *
         * @return const vk::CommandPool&
         */
        const vk::CommandPool &get_handle() const;

        /**
         * @brief Get the queue family of this allocator
         *
         * @return QueueFamily
         */
        QueueFamily get_family() const;

        /**
         * @brief Allocate a command buffer
         *
         * @param level Command buffer level
         * @param count Number of buffers to allocate
         * @return std::vector<vk::UniqueCommandBuffer>
         */
        std::vector<vk::UniqueCommandBuffer>
        allocate(vk::CommandBufferLevel level, unsigned count);

        /**
         * @brief Reset the command pool
         *
         * @param flags
         */
        void reset(vk::CommandPoolResetFlags flags);
    };
} // namespace Dynamo::Graphics::Vulkan