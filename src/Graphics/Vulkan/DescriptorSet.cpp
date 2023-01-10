#include "./DescriptorSet.hpp"

namespace Dynamo::Graphics::Vulkan {
    DescriptorSet::DescriptorSet(Device &device, vk::DescriptorSet &handle) :
        _handle(handle), _device(device) {}

    DescriptorSet::~DescriptorSet() {}

    const vk::DescriptorSet &DescriptorSet::get_handle() const {
        return _handle;
    }
} // namespace Dynamo::Graphics::Vulkan