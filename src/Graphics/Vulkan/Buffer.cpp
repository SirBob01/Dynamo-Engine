#include "./Buffer.hpp"

namespace Dynamo::Graphics::Vulkan {
    Buffer::Buffer(Device &device,
                   u32 size,
                   MemoryPool &memory_pool,
                   CommandPool &command_pool,
                   vk::Queue &transfer_queue,
                   vk::BufferUsageFlags usage,
                   vk::MemoryPropertyFlags properties) :
        _handle(create_raw_buffer(device.get_handle(), size, usage)),
        _device(device), _allocator(size),
        _memory(memory_pool.allocate(get_memory_requirements(), properties)),
        _memory_pool(memory_pool), _command_pool(command_pool) {
        _usage = usage;
        _properties = properties;

        _transfer_queue = transfer_queue;
        _command_buffer = std::move(
            command_pool.allocate(vk::CommandBufferLevel::ePrimary, 1)[0]);

        _memory.bind(_handle);
    }

    Buffer::Buffer(Device &device,
                   MemoryPool &memory_pool,
                   CommandPool &command_pool,
                   vk::Queue &transfer_queue,
                   vk::BufferUsageFlags usage,
                   vk::MemoryPropertyFlags properties) :
        Buffer(device,
               DEFAULT_BUFFER_SIZE,
               memory_pool,
               command_pool,
               transfer_queue,
               usage,
               properties) {}

    Buffer::~Buffer() { _device.get().get_handle().destroyBuffer(_handle); }

    vk::Buffer Buffer::create_raw_buffer(const vk::Device &device,
                                         u32 size,
                                         vk::BufferUsageFlags usage) {
        vk::BufferCreateInfo buffer_info;
        buffer_info.size = size;
        buffer_info.usage = usage | vk::BufferUsageFlagBits::eTransferDst |
                            vk::BufferUsageFlagBits::eTransferSrc;

        return device.createBuffer(buffer_info);
    }

    void Buffer::grow(u32 size) {
        // Allocate new memory and buffer
        vk::Buffer new_handle =
            create_raw_buffer(_device.get().get_handle(), size, _usage);
        MemoryBlock new_memory = _memory_pool.get().allocate(
            _device.get().get_handle().getBufferMemoryRequirements(new_handle),
            _properties);
        new_memory.bind(new_handle);

        // Reset the command pool
        _command_pool.get().reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        // Perform the copy
        vk::CommandBufferBeginInfo begin_info;
        begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        _command_buffer->begin(begin_info);

        vk::BufferCopy copy_region;
        copy_region.dstOffset = 0;
        copy_region.srcOffset = 0;
        copy_region.size = _allocator.capacity();
        _command_buffer->copyBuffer(_handle, new_handle, copy_region);

        _command_buffer->end();

        // Submit the command to the transfer queue
        vk::SubmitInfo submit_info;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_command_buffer.get();

        _transfer_queue.submit(submit_info, nullptr);
        _transfer_queue.waitIdle();

        // Reassign
        _device.get().get_handle().destroyBuffer(_handle);
        _handle = new_handle;
        _memory = std::move(new_memory);

        // Update the allocator
        _allocator.grow(size);
    }

    const vk::Buffer &Buffer::get_handle() const { return _handle; }

    vk::MemoryRequirements Buffer::get_memory_requirements() const {
        return _device.get().get_handle().getBufferMemoryRequirements(_handle);
    }

    u32 Buffer::reserve(u32 size, u32 alignment) {
        std::optional<u32> result = _allocator.reserve(size, alignment);
        if (result.has_value()) {
            return result.value();
        } else {
            u32 current = _allocator.capacity();
            grow(std::max(align_size(current + size, 2), current * 2));

            // If this fails, we're in trouble...
            return _allocator.reserve(size, alignment).value();
        }
    }

    void Buffer::free(u32 offset) { return _allocator.free(offset); }

    void Buffer::write(u8 *src, u32 offset, u32 length) {
        if (!_allocator.is_reserved(offset)) {
            Log::error("Invalid Vulkan buffer offset write");
        }
        _memory.write(src, offset, length);
    }

    void Buffer::read(u8 *dst, u32 offset, u32 length) {
        if (!_allocator.is_reserved(offset)) {
            Log::error("Invalid Vulkan buffer offset read");
        }
        _memory.read(dst, offset, length);
    }

    u32 Buffer::capacity() const { return _allocator.capacity(); }

    u32 Buffer::size(u32 offset) const { return _allocator.size(offset); }

    void Buffer::copy(Buffer &dst, u32 src_offset, u32 dst_offset, u32 length) {
        // Make sure there is enough space
        if (length > dst.size(dst_offset)) {
            Log::error("Attempted to copy Vulkan buffer contents to "
                       "destination block with inadequate size.");
        }

        // Reset the command pool
        _command_pool.get().reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        // Perform the copy
        vk::CommandBufferBeginInfo begin_info;
        begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        _command_buffer->begin(begin_info);

        vk::BufferCopy copy_region;
        copy_region.dstOffset = dst_offset;
        copy_region.srcOffset = src_offset;
        copy_region.size = length;
        _command_buffer->copyBuffer(_handle, dst.get_handle(), copy_region);

        _command_buffer->end();

        // Submit the command to the transfer queue
        vk::SubmitInfo submit_info;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_command_buffer.get();

        _transfer_queue.submit(submit_info, nullptr);
        _transfer_queue.waitIdle();
    }
} // namespace Dynamo::Graphics::Vulkan