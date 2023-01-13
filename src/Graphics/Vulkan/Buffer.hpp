#pragma once

#include <optional>

#include <vulkan/vulkan.hpp>

#include "../../Utils/Allocator.hpp"
#include "./CommandPool.hpp"
#include "./Device.hpp"
#include "./MemoryPool.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Initial size of a Vulkan buffer
     *
     */
    constexpr unsigned DEFAULT_BUFFER_SIZE = (1 << 20);

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
                                            unsigned size,
                                            vk::BufferUsageFlags usage);

        /**
         * @brief Grow the buffer
         *
         * @param size
         */
        void grow(unsigned size);

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
               unsigned size,
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
         * @return unsigned
         */
        unsigned reserve(unsigned size, unsigned alignment);

        /**
         * @brief Free the block of reserved memory at an offset
         *
         * @param offset Offset within the buffer in bytes returned by reserve()
         */
        void free(unsigned offset);

        /**
         * @brief Get the total capacity of the buffer
         *
         * @return unsigned
         */
        unsigned capacity() const;

        /**
         * @brief Get the size of a reserved memory block
         *
         * @param offset Offset within the buffer in bytes returned by reserve()
         */
        unsigned size(unsigned offset) const;

        /**
         * @brief Write to the underlying memory
         *
         * @param src    Source buffer
         * @param offset Offset within the buffer in bytes returned by reserve()
         * @param length Length of the write in bytes
         */
        void write(char *src, unsigned offset, unsigned length);

        /**
         * @brief Read from the underlying memory
         *
         * @param dst    Destination buffer
         * @param offset Offset within the buffer in bytes returned by reserve()
         * @param length Length of the read in bytes
         */
        void read(char *dst, unsigned offset, unsigned length);

        /**
         * @brief Copy the contents of a buffer block to another bufffer
         *
         * @param dst        Destination buffer
         * @param src_offset Valid source offset
         * @param dst_offset Valid destination offset
         * @param length     Length of the copy <= destination block size
         */
        void copy(Buffer &dst,
                  unsigned src_offset,
                  unsigned dst_offset,
                  unsigned length);
    };
} // namespace Dynamo::Graphics::Vulkan