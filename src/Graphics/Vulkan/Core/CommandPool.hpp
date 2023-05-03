#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "../../../Types.hpp"
#include "./Device.hpp"
#include "./PhysicalDevice.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan command pool. This allocates command
     * buffer sets of a given level.
     *
     */
    class CommandPool {
        vk::CommandPool _handle;
        std::reference_wrapper<Device> _device;
        QueueFamily _family;

      public:
        /**
         * @brief Construct a new CommandPool object.
         *
         * @param device Reference to the logical device
         * @param family Queue family
         */
        CommandPool(Device &device, QueueFamily family);

        /**
         * @brief Destroy the CommandPool object.
         *
         */
        ~CommandPool();

        /**
         * @brief Get the handle to vk::CommandPool.
         *
         * @return const vk::CommandPool&
         */
        const vk::CommandPool &get_handle() const;

        /**
         * @brief Get the queue family of this allocator.
         *
         * @return QueueFamily
         */
        QueueFamily get_family() const;

        /**
         * @brief Allocate a set of command buffers.
         *
         * @param level Command buffer level
         * @param count Number of buffers to allocate
         * @return std::vector<vk::UniqueCommandBuffer>
         */
        std::vector<vk::UniqueCommandBuffer>
        allocate(vk::CommandBufferLevel level, u32 count);

        /**
         * @brief Reset the command pool.
         *
         * @param flags
         */
        void reset(vk::CommandPoolResetFlags flags);
    };
} // namespace Dynamo::Graphics::Vulkan