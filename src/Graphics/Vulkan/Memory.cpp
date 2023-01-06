#include "./Memory.hpp"

namespace Dynamo::Graphics::Vulkan {
    Memory::Memory(Device &device,
                   vk::MemoryRequirements requirements,
                   vk::MemoryPropertyFlagBits properties) :
        _device(device) {
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

    std::optional<unsigned> Memory::recycle_old(unsigned size,
                                                unsigned alignment) {
        MemoryBlock block;
        auto it = _blocks.begin();
        while (it != std::prev(_blocks.end())) {
            MemoryBlock &curr = *it;
            MemoryBlock &next = *std::next(it);

            // Find a big enough gap between existing blocks
            block.offset = align_size(curr.offset + curr.size, alignment);
            block.size = size;
            if (block.offset + block.size <= next.offset) {
                _blocks.insert(std::next(it), block);
                return block.offset;
            }
        }
        return {};
    }

    std::optional<unsigned> Memory::reserve_new(unsigned size,
                                                unsigned alignment) {
        MemoryBlock block;
        if (_blocks.empty()) {
            block.offset = 0;
            block.size = size;
        } else {
            MemoryBlock &last = _blocks.back();
            block.offset = align_size(last.offset + last.size, alignment);
            block.size = size;
        }
        if (block.offset + block.size <= _capacity) {
            _blocks.push_back(block);
            return block.offset;
        }
        return {};
    }

    const vk::DeviceMemory &Memory::get_handle() const { return _handle; }

    const vk::MemoryType &Memory::get_type() const { return _type; }

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

    std::optional<unsigned> Memory::reserve(unsigned size, unsigned alignment) {
        std::optional<unsigned> offset = recycle_old(size, alignment);
        if (!offset.has_value()) {
            offset = reserve_new(size, alignment);
        }
        return offset;
    }

    void Memory::free(unsigned offset) {
        auto it = _blocks.begin();
        while (it != _blocks.end()) {
            if (it->offset == offset) {
                _blocks.erase(it);
                return;
            }
            it++;
        }

        // This should never happen, free only valid offsets
        DYN_ASSERT(false);
    }
} // namespace Dynamo::Graphics::Vulkan