#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    Device::Device(PhysicalDevice &physical) : _physical(physical) {
        // Get all unique queue families
        auto cmp = [](QueueProperties a, QueueProperties b) {
            return a.family_id != b.family_id;
        };
        std::set<QueueProperties, decltype(cmp)> unique_families(cmp);
        unique_families.insert(
            _physical.get_queue_properties(QueueFamily::Graphics));
        unique_families.insert(
            _physical.get_queue_properties(QueueFamily::Transfer));
        unique_families.insert(
            _physical.get_queue_properties(QueueFamily::Present));

        // Allocate queues
        std::vector<vk::DeviceQueueCreateInfo> queue_infos;
        std::vector<std::vector<float>> priorities;
        for (const QueueProperties &queue_properties : unique_families) {
            // Priorities influence scheduling command buffer execution
            priorities.emplace_back(queue_properties.count, 0.0f);

            vk::DeviceQueueCreateInfo queue_info;
            queue_info.queueFamilyIndex = queue_properties.family_id;
            queue_info.queueCount = queue_properties.count;
            queue_info.pQueuePriorities = priorities.back().data();

            queue_infos.push_back(queue_info);
        }

        // Enable certain features of the physical device
        vk::PhysicalDeviceFeatures device_features;
        device_features.samplerAnisotropy = true;
        device_features.sampleRateShading = true;
        device_features.fillModeNonSolid = true;

        vk::PhysicalDeviceDescriptorIndexingFeatures descriptor_indexing;
        descriptor_indexing.descriptorBindingPartiallyBound = true;
        descriptor_indexing.runtimeDescriptorArray = true;
        descriptor_indexing.descriptorBindingVariableDescriptorCount = true;

        // Create the logical device
        auto &extensions = _physical.get_extensions();

        vk::DeviceCreateInfo device_info;
        device_info.queueCreateInfoCount = queue_infos.size();
        device_info.pQueueCreateInfos = queue_infos.data();
        device_info.enabledExtensionCount = extensions.size();
        device_info.ppEnabledExtensionNames = extensions.data();
        device_info.pEnabledFeatures = &device_features;
        device_info.pNext = &descriptor_indexing;

        _handle = _physical.get_handle().createDevice(device_info);
    }

    Device::~Device() {
        wait();
        _handle.destroy();
    }

    const vk::Device &Device::get_handle() const { return _handle; }

    PhysicalDevice &Device::get_physical() { return _physical; }

    vk::Queue Device::get_queue(QueueFamily family, unsigned index) {
        QueueProperties properties = _physical.get_queue_properties(family);
        return _handle.getQueue(properties.family_id, index);
    }

    void Device::wait() { _handle.waitIdle(); }
} // namespace Dynamo::Graphics::Vulkan