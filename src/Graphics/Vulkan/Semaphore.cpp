#include "./Semaphore.hpp"

namespace Dynamo::Graphics::Vulkan {
    Semaphore::Semaphore(Device &device) : _device(device) {
        vk::SemaphoreCreateInfo semaphore_info;
        _handle = _device.get().get_handle().createSemaphore(semaphore_info);
    }

    Semaphore::~Semaphore() {
        _device.get().get_handle().destroySemaphore(_handle);
    }

    const vk::Semaphore &Semaphore::get_handle() const { return _handle; }
} // namespace Dynamo::Graphics::Vulkan