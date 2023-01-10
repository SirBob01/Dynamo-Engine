#include "./Fence.hpp"

namespace Dynamo::Graphics::Vulkan {
    Fence::Fence(Device &device) : _device(device) {
        vk::FenceCreateInfo fence_info;
        fence_info.flags = vk::FenceCreateFlagBits::eSignaled;

        _handle = _device.get().get_handle().createFence(fence_info);
    }

    Fence::~Fence() { _device.get().get_handle().destroyFence(_handle); }

    const vk::Fence &Fence::get_handle() const { return _handle; }

    void Fence::wait(unsigned long timeout) {
        vk::Result result =
            _device.get().get_handle().waitForFences(_handle, true, timeout);
        if (result != vk::Result::eSuccess) {
            Log::error("An error occurred while waiting for Vulkan fence: {}",
                       vk::to_string(result));
        }
    }

    bool Fence::is_signaled() {
        return _device.get().get_handle().getFenceStatus(_handle) ==
               vk::Result::eSuccess;
    }

    void Fence::reset() { _device.get().get_handle().resetFences(_handle); }
} // namespace Dynamo::Graphics::Vulkan