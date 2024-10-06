#include <Graphics/Vulkan/Image.hpp>

namespace Dynamo::Graphics::Vulkan {
    Image::Image(Device &device, VkImage handle, VkFormat format) :
        _device(device), _handle(handle), _format(format) {
        vkGetImageMemoryRequirements(device.handle(),
                                     _handle,
                                     &_memory_requirements);
    }

    VkImage Image::handle() const { return _handle; }

    VkFormat Image::format() const { return _format; }

    VkMemoryRequirements Image::memory_requirements() const {
        return _memory_requirements;
    }

    Device &Image::device() const { return _device; }
} // namespace Dynamo::Graphics::Vulkan