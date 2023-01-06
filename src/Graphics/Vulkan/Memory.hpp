#pragma once

#include <functional>
#include <list>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Round up a size to be a multiple of alignment
     *
     * @param size      Size in bytes
     * @param alignment Alignment in bytes
     * @return unsigned
     */
    inline unsigned align_size(unsigned size, unsigned alignment) {
        return ((size + alignment - 1) / alignment) * alignment;
    }

    /**
     * @brief Block of reserved memory
     *
     */
    struct MemoryBlock {
        /**
         * @brief Offset of the block within the allocated memory
         *
         */
        unsigned offset;

        /**
         * @brief Size of the block in bytes
         *
         */
        unsigned size;
    };

    /**
     * @brief Wrapper class for Vulkan device memory that implements the
     * block allocation and deallocation algorithms
     *
     * TODO: Improve allocator runtime complexity
     *
     */
    class Memory {
        vk::DeviceMemory _handle;
        std::reference_wrapper<Device> _device;

        vk::MemoryType _type;
        unsigned _capacity;

        char *_mapped;

        std::list<MemoryBlock> _blocks;

        /**
         * @brief Recycle an existing block
         *
         * @param size
         * @param offset
         * @return std::optional<unsigned>
         */
        std::optional<unsigned> recycle_old(unsigned size, unsigned offset);

        /**
         * @brief Reserve a new block
         *
         * @param size
         * @param alignment
         * @return std::optional<unsigned>
         */
        std::optional<unsigned> reserve_new(unsigned size, unsigned alignment);

      public:
        /**
         * @brief Construct a new Memory object
         *
         * @param device       Reference to the logical device
         * @param requirements Memory allocation requirements
         * @param properties   Memory properties
         */
        Memory(Device &device,
               vk::MemoryRequirements requirements,
               vk::MemoryPropertyFlagBits properties);

        /**
         * @brief Destroy the Memory object
         *
         */
        ~Memory();

        /**
         * @brief Get the handle to vk::DeviceMemory
         *
         * @return const vk::DeviceMemory&
         */
        const vk::DeviceMemory &get_handle() const;

        /**
         * @brief Get the memory type
         *
         * @return const vk::MemoryType&
         */
        const vk::MemoryType &get_type() const;

        /**
         * @brief Get the total capacity of the memory
         *
         * @return unsigned
         */
        unsigned get_capacity() const;

        /**
         * @brief Read from mapped memory
         *
         * @param dst    Destination buffer
         * @param offset Offset within the memory map
         * @param length Length of the read
         */
        void read(char *dst, unsigned offset, unsigned length);

        /**
         * @brief Write to mapped memory
         *
         * @param src    Source buffer
         * @param offset Offset within the memory map
         * @param length Length of the write
         */
        void write(char *src, unsigned offset, unsigned length);

        /**
         * @brief Reserve a block of memory with specific alignment
         * requirements, returning the offset within the pool
         *
         * @param size      Desired size in bytes
         * @param alignment Alignment requirement in bytes
         * @return std::optional<unsigned>
         */
        std::optional<unsigned> reserve(unsigned size, unsigned alignment);

        /**
         * @brief Free the block of reserved memory at an offset
         *
         * @param offset Offset within the pool in bytes returned by reserve()
         */
        void free(unsigned offset);
    };
} // namespace Dynamo::Graphics::Vulkan
