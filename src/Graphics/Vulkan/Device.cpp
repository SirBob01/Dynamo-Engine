#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    static const char *VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME =
        "VK_KHR_portability_subset";
    static std::array<const char *, 2> EXTENSIONS = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME,
    };

    void build_queue_infos(const PhysicalDevice &physical,
                           VkDeviceQueueCreateInfo *dst,
                           unsigned *count) {
        std::array<std::reference_wrapper<const QueueFamily>, 4> families = {
            physical.graphics_queues,
            physical.present_queues,
            physical.transfer_queues,
            physical.compute_queues,
        };

        // Build unique queue create info structures
        *count = 0;
        for (const QueueFamily family : families) {
            VkDeviceQueueCreateInfo queue_info = {};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueCount = family.count;
            queue_info.queueFamilyIndex = family.index;
            queue_info.pQueuePriorities = &family.priority;

            bool found = false;
            for (unsigned i = 0; i < *count; i++) {
                if (dst[i].queueFamilyIndex == family.index) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                dst[(*count)++] = queue_info;
            }
        }
    }

    bool requires_portability(const PhysicalDevice &device) {
        // Enumerate device extensions
        unsigned count = 0;
        vkEnumerateDeviceExtensionProperties(device.handle,
                                             nullptr,
                                             &count,
                                             nullptr);
        std::vector<VkExtensionProperties> extensions(count);
        vkEnumerateDeviceExtensionProperties(device.handle,
                                             nullptr,
                                             &count,
                                             extensions.data());

        // Check if portability subset extension is available
        for (const VkExtensionProperties &extension : extensions) {
            if (!std::strcmp(extension.extensionName,
                             VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME)) {
                return true;
            }
        }
        return false;
    }

    VkDevice VkDevice_build(const PhysicalDevice &physical) {
        // Enable physical device features
        VkPhysicalDeviceFeatures device_features = {};
        device_features.samplerAnisotropy = true;
        device_features.sampleRateShading = true;
        device_features.fillModeNonSolid = true;
        device_features.multiViewport = true;

        // Enable descriptor indexing features
        VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing = {};
        descriptor_indexing.sType =
            VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
        descriptor_indexing.descriptorBindingPartiallyBound = true;
        descriptor_indexing.runtimeDescriptorArray = true;
        descriptor_indexing.descriptorBindingVariableDescriptorCount = true;

        // Get all required device queues
        std::array<VkDeviceQueueCreateInfo, 4> queue_infos;
        unsigned count;
        build_queue_infos(physical, queue_infos.data(), &count);

        VkDeviceCreateInfo device_info = {};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.queueCreateInfoCount = count;
        device_info.pQueueCreateInfos = queue_infos.data();
        device_info.enabledExtensionCount = 1 + requires_portability(physical);
        device_info.ppEnabledExtensionNames = EXTENSIONS.data();
        device_info.pEnabledFeatures = &device_features;
        device_info.pNext = &descriptor_indexing;

        VkDevice device;
        VkResult_log(
            "Create Device",
            vkCreateDevice(physical.handle, &device_info, nullptr, &device));
        return device;
    }

    VkQueue
    VkDevice_queue(VkDevice device, const QueueFamily &family, unsigned index) {
        DYN_ASSERT(index < family.count);
        VkQueue queue;
        vkGetDeviceQueue(device, family.index, index, &queue);
        return queue;
    }
} // namespace Dynamo::Graphics::Vulkan