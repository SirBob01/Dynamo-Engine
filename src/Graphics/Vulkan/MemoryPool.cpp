#include "./MemoryPool.hpp"

namespace Dynamo::Graphics::Vulkan {
    MemoryBlock::MemoryBlock(Memory &memory, unsigned offset, unsigned size) :
        _memory(memory), _offset(offset), _size(size), _moved(false) {}

    MemoryBlock::MemoryBlock(MemoryBlock &&rhs) :
        _memory(rhs._memory), _offset(rhs._offset), _size(rhs._size) {
        rhs._moved = true;
    }

    MemoryBlock::~MemoryBlock() {
        if (!_moved) {
            _memory.get().free(_offset);
        }
    }

    const vk::DeviceMemory &MemoryBlock::get_handle() const {
        DYN_ASSERT(!_moved);
        return _memory.get().get_handle();
    }

    unsigned MemoryBlock::offset() const {
        DYN_ASSERT(!_moved);
        return _offset;
    }

    unsigned MemoryBlock::size() const {
        DYN_ASSERT(!_moved);
        return _size;
    }

    void MemoryBlock::read(char *dst, unsigned offset, unsigned length) {
        DYN_ASSERT(!_moved);
        _memory.get().read(dst, offset + _offset, length);
    }

    void MemoryBlock::write(char *src, unsigned offset, unsigned length) {
        DYN_ASSERT(!_moved);
        _memory.get().write(src, offset + _offset, length);
    }

    void MemoryBlock::bind(vk::Image vkimage) {
        DYN_ASSERT(!_moved);
        _memory.get().bind(vkimage, _offset);
    }

    void MemoryBlock::bind(vk::Buffer vkbuffer) {
        DYN_ASSERT(!_moved);
        _memory.get().bind(vkbuffer, _offset);
    }

    MemoryPool::MemoryPool(Device &device) : _device(device) {}

    bool MemoryPool::is_compatible(Memory &memory,
                                   vk::MemoryRequirements requirements,
                                   vk::MemoryPropertyFlags properties) {
        const vk::PhysicalDeviceMemoryProperties &physical_memory =
            _device.get().get_physical().get_memory_properties();
        unsigned index;
        for (index = 0; index < physical_memory.memoryTypeCount; index++) {
            vk::MemoryType type = physical_memory.memoryTypes[index];
            if ((requirements.memoryTypeBits & (1 << index)) &&
                (memory.get_type() == type) &&
                ((type.propertyFlags & properties) == properties)) {
                return true;
            }
        }
        return false;
    }

    MemoryBlock MemoryPool::allocate(vk::MemoryRequirements requirements,
                                     vk::MemoryPropertyFlags properties) {
        // Look for an existing compatible pool
        for (std::unique_ptr<Memory> &memory : _pools) {
            if (is_compatible(*memory, requirements, properties)) {
                std::optional<unsigned> result =
                    memory.get()->reserve(requirements.size,
                                          requirements.alignment);
                if (result.has_value()) {
                    return MemoryBlock(std::ref(*memory),
                                       result.value(),
                                       requirements.size);
                }
            }
        }

        // Create a new memory pool
        vk::MemoryRequirements alloc_requirements;
        alloc_requirements.size =
            std::max(align_size(requirements.size, requirements.alignment),
                     MINIMUM_MEMORY_CAPACITY);
        alloc_requirements.memoryTypeBits = requirements.memoryTypeBits;

        _pools.push_back(std::make_unique<Memory>(_device.get(),
                                                  alloc_requirements,
                                                  properties));
        std::unique_ptr<Memory> &memory = _pools.back();

        // Allocate memory
        std::optional<unsigned> result =
            memory.get()->reserve(requirements.size, requirements.alignment);
        if (!result.has_value()) {
            Log::error("Unable to allocate Vulkan memory");
        }
        return MemoryBlock(std::ref(*memory),
                           result.value(),
                           requirements.size);
    }

    void MemoryBlock::operator=(MemoryBlock &&rhs) {
        _memory = rhs._memory;
        _offset = rhs._offset;
        _size = rhs._size;
        rhs._moved = true;
    }
} // namespace Dynamo::Graphics::Vulkan