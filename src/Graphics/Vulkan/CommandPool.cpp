#include "./CommandPool.hpp"

namespace Dynamo::Graphics::Vulkan {
    CommandPool::CommandPool(Device &device, QueueFamily family) :
        _device(device), _family(family) {
        vk::CommandPoolCreateInfo pool_info;
        pool_info.queueFamilyIndex =
            _device.get().get_physical().get_queue_properties(family).family_id;
        _handle = _device.get().get_handle().createCommandPool(pool_info);
    }

    CommandPool::~CommandPool() {
        _device.get().get_handle().destroyCommandPool(_handle);
    }

    const vk::CommandPool &CommandPool::get_handle() const {
        return _handle;
    }

    QueueFamily CommandPool::get_family() const { return _family; }

    std::vector<vk::UniqueCommandBuffer>
    CommandPool::allocate(vk::CommandBufferLevel level, unsigned count) {
        vk::CommandBufferAllocateInfo alloc_info;
        alloc_info.commandPool = _handle;
        alloc_info.level = level;
        alloc_info.commandBufferCount = count;

        return _device.get().get_handle().allocateCommandBuffersUnique(
            alloc_info);
    }

    void CommandPool::reset(vk::CommandPoolResetFlags flags) {
        _device.get().get_handle().resetCommandPool(_handle, flags);
    }
} // namespace Dynamo::Graphics::Vulkan