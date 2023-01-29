#pragma once

#include <optional>

#include <vulkan/vulkan.hpp>

#include "../../Types.hpp"
#include "../../Utils/Allocator.hpp"
#include "./CommandPool.hpp"
#include "./Device.hpp"
#include "./MemoryPool.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Initial size of a Vulkan buffer
     *
     */
    constexpr u32 DEFAULT_BUFFER_SIZE = (1 << 20);

    /**
     * @brief Wrapper class of Vulkan Buffer
     *
     */
    class Buffer {
        vk::Buffer _handle;
        std::reference_wrapper<Device> _device;
        Allocator _allocator;
        MemoryBlock _memory;

        std::reference_wrapper<MemoryPool> _memory_pool;
        std::reference_wrapper<CommandPool> _command_pool;

        vk::BufferUsageFlags _usage;
        vk::MemoryPropertyFlags _properties;

        vk::Queue _transfer_queue;
        vk::UniqueCommandBuffer _command_buffer;

        /**
         * @brief Create a vk::Buffer handle
         *
         * @param device vk::Device handle
         * @param size   Size of the buffer
         * @param usage  Buffer usage flags
         * @return vk::Buffer
         */
        static vk::Buffer create_raw_buffer(const vk::Device &device,
                                            u32 size,
                                            vk::BufferUsageFlags usage);

        /**
         * @brief Grow the buffer
         *
         * @param size
         */
        void grow(u32 size);

      public:
        /**
         * @brief Construct a new Buffer object
         *
         * @param device         Reference to the logical device
         * @param size           Initial size of the buffer
         * @param memory_pool    Reference to the memory pool
         * @param command_pool   Reference to the command pool
         * @param transfer_queue Transfer command queue
         * @param usage          Buffer usage flags
         * @param properties     Underlying buffer memory propertes
         */
        Buffer(Device &device,
               u32 size,
               MemoryPool &memory_pool,
               CommandPool &command_pool,
               vk::Queue &transfer_queue,
               vk::BufferUsageFlags usage,
               vk::MemoryPropertyFlags properties);

        /**
         * @brief Construct a new Buffer object
         *
         * @param device         Reference to the logical device
         * @param memory_pool    Reference to the memory pool
         * @param command_pool   Reference to the command pool
         * @param transfer_queue Transfer command queue
         * @param usage          Buffer usage flags
         * @param properties     Underlying buffer memory propertes
         */
        Buffer(Device &device,
               MemoryPool &memory_pool,
               CommandPool &command_pool,
               vk::Queue &transfer_queue,
               vk::BufferUsageFlags usage,
               vk::MemoryPropertyFlags properties);

        /**
         * @brief Destroy the Buffer object
         *
         */
        ~Buffer();

        /**
         * @brief Get the handle to vk::Buffer
         *
         * @return const vk::Buffer&
         */
        const vk::Buffer &get_handle() const;

        /**
         * @brief Get the memory requirements for the buffer
         *
         * @return vk::MemoryRequirements
         */
        vk::MemoryRequirements get_memory_requirements() const;

        /**
         * @brief Reserve a block of memory with specific alignment
         * requirements, returning the offset within the buffer.
         *
         * @param size      Desired size in bytes
         * @param alignment Alignment requirement in bytes
         * @return u32
         */
        u32 reserve(u32 size, u32 alignment);

        /**
         * @brief Free the block of reserved memory at an offset
         *
         * @param offset Offset within the buffer in bytes returned by reserve()
         */
        void free(u32 offset);

        /**
         * @brief Get the total capacity of the buffer
         *
         * @return u32
         */
        u32 capacity() const;

        /**
         * @brief Get the size of a reserved memory block
         *
         * @param offset Offset within the buffer in bytes returned by reserve()
         */
        u32 size(u32 offset) const;

        /**
         * @brief Write to the underlying memory
         *
         * @param src    Source buffer
         * @param offset Offset within the buffer in bytes returned by reserve()
         * @param length Length of the write in bytes
         */
        void write(i8 *src, u32 offset, u32 length);

        /**
         * @brief Read from the underlying memory
         *
         * @param dst    Destination buffer
         * @param offset Offset within the buffer in bytes returned by reserve()
         * @param length Length of the read in bytes
         */
        void read(i8 *dst, u32 offset, u32 length);

        /**
         * @brief Copy the contents of a buffer block to another bufffer
         *
         * @param dst        Destination buffer
         * @param src_offset Valid source offset
         * @param dst_offset Valid destination offset
         * @param length     Length of the copy <= destination block size
         */
        void copy(Buffer &dst,
                  u32 src_offset,
                  u32 dst_offset,
                  u32 length);
    };
} // namespace Dynamo::Graphics::Vulkan