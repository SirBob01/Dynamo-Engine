#include "./Allocator.hpp"

namespace Dynamo::Graphics::Vulkan {
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

    std::optional<Allocation>
    Allocator::allocate(vk::MemoryRequirements requirements,
                        vk::MemoryPropertyFlagBits properties) {
        // Look for an existing compatible pool
        for (std::unique_ptr<Memory> &memory : _pools) {
            if (is_compatible(*memory, requirements, properties)) {
                std::optional<unsigned> offset =
                    memory.get()->reserve(requirements.size,
                                          requirements.alignment);
                if (offset.has_value()) {
                    return std::optional<Allocation>(
                        {std::ref(*memory), offset.value()});
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
        std::optional<unsigned> offset =
            memory.get()->reserve(requirements.size, requirements.alignment);
        return std::optional<Allocation>({std::ref(*memory), offset.value()});
    }
} // namespace Dynamo::Graphics::Vulkan