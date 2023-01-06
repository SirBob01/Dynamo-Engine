#pragma once

#include <functional>
#include <optional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Utils/Bits.hpp"
#include "./Device.hpp"
#include "./Memory.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Minimum capacity of a Memory object
     *
     */
    constexpr unsigned MINIMUM_MEMORY_CAPACITY = 256 * (1 << 20);

    /**
     * @brief Allocation exposes a similar interface to Memory for managing an
     * underlying block of memory at an offset
     *
     */
    class Allocation {
        /**
         * @brief Reference to the memory
         *
         */
        std::reference_wrapper<Memory> _memory;

        /**
         * @brief Reserved offset within the memory
         *
         */
        unsigned _offset;

        /**
         * @brief Size of the reserveed block
         *
         */
        unsigned _size;

      public:
        /**
         * @brief Construct a new Allocation object
         *
         * @param memory Reference to the memory
         * @param offset Reserved block offset in bytes
         * @param size   Reserved block size in bytes
         */
        Allocation(Memory &memory, unsigned offset, unsigned size);

        /**
         * @brief Move constructor
         *
         * @param rhs
         */
        Allocation(Allocation &&rhs);

        /**
         * @brief Copy constructor
         *
         * @param rhs
         */
        Allocation(Allocation &rhs) = delete;

        /**
         * @brief Destroy the Allocation object
         *
         */
        ~Allocation();

        /**
         * @brief Get the handle to vk::DeviceMemory
         *
         * @return const vk::DeviceMemory&
         */
        const vk::DeviceMemory &get_handle() const;

        /**
         * @brief Get the offset
         *
         * @return unsigned
         */
        unsigned offset() const;

        /**
         * @brief Get the size of the allocation
         *
         * @return unsigned
         */
        unsigned size() const;

        /**
         * @brief Read from the underlying memory at an offset relative to this
         * allocation
         *
         * @param dst    Destination buffer
         * @param offset Offset within the memory map in bytes
         * @param length Length of the read in bytes
         */
        void read(char *dst, unsigned offset, unsigned length);

        /**
         * @brief Write to the underlying memory at an offset relative to this
         * allocation
         *
         * @param dst    Source buffer
         * @param offset Offset within the memory map in bytes
         * @param length Length of the read in bytes
         */
        void write(char *src, unsigned offset, unsigned length);

        /**
         * @brief Bind a vk::Image to the memory
         *
         * @param vkimage
         */
        void bind(vk::Image vkimage);

        /**
         * @brief Bind a vk::Buffer to the memory
         *
         * @param vkbuffer
         */
        void bind(vk::Buffer vkbuffer);
    };

    /**
     * @brief Dynamic memory allocator
     *
     * Memory objects are grouped based on their type so allocations with
     * desired properties can be queried
     *
     */
    class Allocator {
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
        bool is_compatible(Memory &memory,
                           vk::MemoryRequirements requirements,
                           vk::MemoryPropertyFlagBits properties);

      public:
        /**
         * @brief Construct a new Allocator object
         *
         * @param device Reference to the logical device
         */
        Allocator(Device &device);

        /**
         * @brief Allocate a block of memory with specific requirements and
         * properties
         *
         * @param requirements Memory requirements
         * @param properties   Memory properties
         * @return Allocation
         */
        Allocation allocate(vk::MemoryRequirements requirements,
                            vk::MemoryPropertyFlagBits properties);
    };
} // namespace Dynamo::Graphics::Vulkan