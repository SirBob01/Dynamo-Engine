#include <Graphics/Vulkan/Device.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    Device::Device(PhysicalDevice &physical) : _physical(physical) {
        // Enable certain features of the physical device
        VkPhysicalDeviceFeatures device_features = {};
        device_features.samplerAnisotropy = true;
        device_features.sampleRateShading = true;
        device_features.fillModeNonSolid = true;
        device_features.multiViewport = true;

        VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing = {};
        descriptor_indexing.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
        descriptor_indexing.descriptorBindingPartiallyBound = true;
        descriptor_indexing.runtimeDescriptorArray = true;
        descriptor_indexing.descriptorBindingVariableDescriptorCount = true;

        // Get all required device extensions and queues
        const std::vector<const char *> &extensions =
            _physical.required_extensions();
        const std::vector<VkDeviceQueueCreateInfo> queue_infos =
            get_queue_infos();

        VkDeviceCreateInfo device_info = {};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.queueCreateInfoCount = queue_infos.size();
        device_info.pQueueCreateInfos = queue_infos.data();
        device_info.enabledExtensionCount = extensions.size();
        device_info.ppEnabledExtensionNames = extensions.data();
        device_info.pEnabledFeatures = &device_features;
        device_info.pNext = &descriptor_indexing;

        VkResult result =
            vkCreateDevice(_physical.handle(), &device_info, nullptr, &_handle);
        if (result != VK_SUCCESS) {
            Log::error("Unable to create Vulkan::Device.");
        }
    }

    Device::~Device() { vkDestroyDevice(_handle, nullptr); }

    std::vector<VkDeviceQueueCreateInfo> Device::get_queue_infos() const {
        std::array<std::reference_wrapper<const QueueFamily>, 4> families = {
            _physical.graphics_queues(),
            _physical.present_queues(),
            _physical.transfer_queues(),
            _physical.compute_queues(),
        };
        std::vector<VkDeviceQueueCreateInfo> queue_infos;

        // Build unique queue create info structures
        for (const QueueFamily family : families) {
            VkDeviceQueueCreateInfo queue_info = {};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueCount = family.count;
            queue_info.queueFamilyIndex = family.index;
            queue_info.pQueuePriorities = &family.priority;

            bool found = false;
            for (VkDeviceQueueCreateInfo &info : queue_infos) {
                if (info.queueFamilyIndex == family.index) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                queue_infos.push_back(queue_info);
            }
        }

        return queue_infos;
    }

    VkDevice Device::handle() const { return _handle; }

    const PhysicalDevice &Device::physical() const { return _physical; }

    VkQueue Device::queue(QueueType type, unsigned index) const {
        QueueFamily family;
        switch (type) {
        case QueueType::Graphics:
            family = _physical.graphics_queues();
            break;
        case QueueType::Present:
            family = _physical.present_queues();
            break;
        case QueueType::Transfer:
            family = _physical.transfer_queues();
            break;
        case QueueType::Compute:
            family = _physical.compute_queues();
            break;
        }

        if (family.index >= family.count) {
            Log::error("Vulkan::Device requested unavailable {} queues.",
                       (int)type);
        }

        VkQueue queue;
        vkGetDeviceQueue(_handle, family.index, index, &queue);
        return queue;
    }

    void Device::await() const { vkDeviceWaitIdle(_handle); }
} // namespace Dynamo::Graphics::Vulkan