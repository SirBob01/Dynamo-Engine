#include "./Memory.hpp"

namespace Dynamo::Graphics::Vulkan {
    Memory::Memory(Device &device,
                   vk::MemoryRequirements requirements,
                   vk::MemoryPropertyFlags properties) :
        _device(device),
        _allocator(requirements.size) {
        // Find the appropriate type index
        const vk::PhysicalDeviceMemoryProperties &physical_memory =
            _device.get().get_physical().get_memory_properties();
        unsigned index;
        for (index = 0; index < physical_memory.memoryTypeCount; index++) {
            _type = physical_memory.memoryTypes[index];
            if ((requirements.memoryTypeBits & (1 << index)) &&
                ((_type.propertyFlags & properties) == properties)) {
                break;
            }
        }

        // Allocate the memory
        vk::MemoryAllocateInfo alloc_info;
        alloc_info.allocationSize = requirements.size;
        alloc_info.memoryTypeIndex = index;

        _handle = _device.get().get_handle().allocateMemory(alloc_info);
        _capacity = requirements.size;

        // Map memory once if possible
        if (properties & vk::MemoryPropertyFlagBits::eHostVisible) {
            _mapped = reinterpret_cast<char *>(
                _device.get().get_handle().mapMemory(_handle, 0, _capacity));
        } else {
            _mapped = nullptr;
        }
    }

    Memory::~Memory() {
        if (_mapped != nullptr) {
            _device.get().get_handle().unmapMemory(_handle);
        }
        _device.get().get_handle().freeMemory(_handle);
    }

    const vk::DeviceMemory &Memory::get_handle() const { return _handle; }

    const vk::MemoryType &Memory::get_type() const { return _type; }

    Device &Memory::get_device() { return _device; }

    unsigned Memory::get_capacity() const { return _capacity; }

    void Memory::read(char *dst, unsigned offset, unsigned length) {
        DYN_ASSERT(_mapped != nullptr);
        DYN_ASSERT(offset + length <= _capacity);
        std::memcpy(dst, _mapped + offset, length);
    }

    void Memory::write(char *src, unsigned offset, unsigned length) {
        DYN_ASSERT(_mapped != nullptr);
        DYN_ASSERT(offset + length <= _capacity);
        std::memcpy(_mapped + offset, src, length);
    }

    void Memory::bind(vk::Image vkimage, unsigned offset) {
        _device.get().get_handle().bindImageMemory(vkimage, _handle, offset);
    }

    void Memory::bind(vk::Buffer vkbuffer, unsigned offset) {
        _device.get().get_handle().bindBufferMemory(vkbuffer, _handle, offset);
    }

    std::optional<unsigned> Memory::reserve(unsigned size, unsigned alignment) {
        return _allocator.reserve(size, alignment);
    }

    void Memory::free(unsigned offset) { return _allocator.free(offset); }
} // namespace Dynamo::Graphics::Vulkan