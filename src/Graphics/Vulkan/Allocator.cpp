#include "./Allocator.hpp"

namespace Dynamo::Graphics::Vulkan {
    Allocation::Allocation(Memory &memory, unsigned offset, unsigned size) :
        _memory(memory), _offset(offset), _size(size) {}

    Allocation::Allocation(Allocation &&rhs) :
        _memory(rhs._memory), _offset(rhs._offset), _size(rhs._size) {}

    Allocation::~Allocation() { _memory.get().free(_offset); }

    const vk::DeviceMemory &Allocation::get_handle() const {
        return _memory.get().get_handle();
    }

    unsigned Allocation::offset() const { return _offset; }

    unsigned Allocation::size() const { return _size; }

    void Allocation::read(char *dst, unsigned offset, unsigned length) {
        _memory.get().read(dst, offset + _offset, length);
    }

    void Allocation::write(char *src, unsigned offset, unsigned length) {
        _memory.get().write(src, offset + _offset, length);
    }

    void Allocation::bind(vk::Image vkimage) {
        _memory.get().bind(vkimage, _offset);
    }

    void Allocation::bind(vk::Buffer vkbuffer) {
        _memory.get().bind(vkbuffer, _offset);
    }

    Allocator::Allocator(Device &device) : _device(device) {}

    bool Allocator::is_compatible(Memory &memory,
                                  vk::MemoryRequirements requirements,
                                  vk::MemoryPropertyFlagBits properties) {
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

    Allocation Allocator::allocate(vk::MemoryRequirements requirements,
                                   vk::MemoryPropertyFlagBits properties) {
        // Look for an existing compatible pool
        for (std::unique_ptr<Memory> &memory : _pools) {
            if (is_compatible(*memory, requirements, properties)) {
                std::optional<unsigned> result =
                    memory.get()->reserve(requirements.size,
                                          requirements.alignment);
                if (result.has_value()) {
                    return Allocation(std::ref(*memory),
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
        return Allocation(std::ref(*memory), result.value(), requirements.size);
    }
} // namespace Dynamo::Graphics::Vulkan