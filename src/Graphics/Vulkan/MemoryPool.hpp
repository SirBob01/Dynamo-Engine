#pragma once

#include <functional>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Types.hpp"
#include "../../Utils/Bits.hpp"
#include "./Device.hpp"
#include "./Memory.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Minimum capacity of a Memory object
     *
     */
    constexpr u32 MINIMUM_MEMORY_CAPACITY = 256 * (1 << 20);

    /**
     * @brief MemoryBlock exposes a similar interface to Memory for managing an
     * underlying block of memory at an offset, but is similar to
     * std::unique_ptr in that it can only be moved and not copied
     *
     */
    class MemoryBlock {
        std::reference_wrapper<Memory> _memory;
        u32 _offset;
        u32 _size;

        b8 _moved;

      public:
        /**
         * @brief Construct a new MemoryBlock object
         *
         * @param memory Reference to the memory
         * @param offset Reserved block offset in bytes
         * @param size   Reserved block size in bytes
         */
        MemoryBlock(Memory &memory, u32 offset, u32 size);

        /**
         * @brief Move constructor
         *
         * @param rhs
         */
        MemoryBlock(MemoryBlock &&rhs);

        /**
         * @brief Destroy the MemoryBlock object
         *
         */
        ~MemoryBlock();

        /**
         * @brief Get the handle to vk::DeviceMemory
         *
         * @return const vk::DeviceMemory&
         */
        const vk::DeviceMemory &get_handle() const;

        /**
         * @brief Get the offset
         *
         * @return u32
         */
        u32 offset() const;

        /**
         * @brief Get the size of the allocation
         *
         * @return u32
         */
        u32 size() const;

        /**
         * @brief Read from the underlying memory at an offset
         *
         * @param dst    Destination buffer
         * @param offset Offset within the memory map in bytes
         * @param length Length of the read in bytes
         */
        void read(i8 *dst, u32 offset, u32 length);

        /**
         * @brief Write to the underlying memory at an offset
         *
         * @param dst    Source buffer
         * @param offset Offset within the memory map in bytes
         * @param length Length of the read in bytes
         */
        void write(i8 *src, u32 offset, u32 length);

        /**
         * @brief Bind a vk::Image to the block
         *
         * @param vkimage
         */
        void bind(vk::Image vkimage);

        /**
         * @brief Bind a vk::Buffer to the block
         *
         * @param vkbuffer
         */
        void bind(vk::Buffer vkbuffer);

        /**
         * @brief Move assignment operator
         *
         * @param rhs
         */
        void operator=(MemoryBlock &&rhs);
    };

    /**
     * @brief Dynamic memory pool
     *
     * Memory objects are grouped based on their type so allocations with
     * desired properties can be queried
     *
     */
    class MemoryPool {
        std::reference_wrapper<Device> _device;

        using Pool = std::vector<std::unique_ptr<Memory>>;
        Pool _pools;

        /**
         * @brief Check if a memory pool is compatible with the given
         * requirements and properties
         *
         * @param memory
         * @param requirements
         * @param properties
         * @return true
         * @return false
         */
        b8 is_compatible(Memory &memory,
                           vk::MemoryRequirements requirements,
                           vk::MemoryPropertyFlags properties);

      public:
        /**
         * @brief Construct a new MemoryPool object
         *
         * @param device Reference to the logical device
         */
        MemoryPool(Device &device);

        /**
         * @brief Allocate a block of memory with specific requirements and
         * properties
         *
         * @param requirements Memory requirements
         * @param properties   Memory properties
         * @return Allocation
         */
        MemoryBlock allocate(vk::MemoryRequirements requirements,
                             vk::MemoryPropertyFlags properties);
    };
} // namespace Dynamo::Graphics::Vulkan