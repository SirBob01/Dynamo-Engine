#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    PhysicalDevice::PhysicalDevice(VkPhysicalDevice handle, Surface &surface) :
        _handle(handle) {
        vkGetPhysicalDeviceProperties(_handle, &_properties);
        vkGetPhysicalDeviceMemoryProperties(_handle, &_memory_properties);
        vkGetPhysicalDeviceFeatures(_handle, &_features);

        // Enumerate device queue families
        unsigned queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(_handle,
                                                 &queue_family_count,
                                                 nullptr);

        _queue_families.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(_handle,
                                                 &queue_family_count,
                                                 _queue_families.data());

        // Select queue families for each type
        unsigned index = 0;
        for (const VkQueueFamilyProperties family : _queue_families) {
            VkBool32 surface_support = 0;
            VkResult result =
                vkGetPhysicalDeviceSurfaceSupportKHR(_handle,
                                                     index,
                                                     surface.handle(),
                                                     &surface_support);
            if (result != VK_SUCCESS) {
                Log::error("Vulkan::PhysicalDevice was unable to query for "
                           "surface support.");
            }

            // Dedicated presentation queues
            if (surface_support && family.queueCount > _present_queues.count) {
                _present_queues.count = family.queueCount;
                _present_queues.index = index;
                _present_queues.priority = 0;
            }

            // Dedicated graphics queues
            if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                family.queueCount > _graphics_queues.count) {
                _graphics_queues.count = family.queueCount;
                _graphics_queues.index = index;
                _graphics_queues.priority = 0;
            }

            // Dedicated transfer queues
            if ((family.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                family.queueCount > _transfer_queues.count) {
                _transfer_queues.count = family.queueCount;
                _transfer_queues.index = index;
                _transfer_queues.priority = 0;
            }

            // Dedicated compute queues
            if ((family.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                family.queueCount > _compute_queues.count) {
                _compute_queues.count = family.queueCount;
                _compute_queues.index = index;
                _compute_queues.priority = 0;
            }
        }

        // Enumerate device extensions
        unsigned ext_count = 0;
        vkEnumerateDeviceExtensionProperties(_handle,
                                             nullptr,
                                             &ext_count,
                                             nullptr);

        _extensions.resize(ext_count);
        vkEnumerateDeviceExtensionProperties(_handle,
                                             nullptr,
                                             &ext_count,
                                             _extensions.data());

        // Set the required device extensions
        _required_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        const char *portability_ext_name = "VK_KHR_portability_subset";
        for (const VkExtensionProperties &extension : _extensions) {
            if (!std::strcmp(extension.extensionName, portability_ext_name)) {
                _required_extensions.push_back(portability_ext_name);
            }
        }
    }

    bool PhysicalDevice::supports_required_extensions() const {
        for (const char *ext_name : _required_extensions) {
            bool found = false;
            for (const VkExtensionProperties &extension : _extensions) {
                if (!std::strcmp(extension.extensionName, ext_name)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                return false;
            }
        }
        return true;
    }

    VkPhysicalDevice PhysicalDevice::handle() const { return _handle; }

    const VkPhysicalDeviceProperties &PhysicalDevice::properties() const {
        return _properties;
    }

    const VkPhysicalDeviceMemoryProperties &
    PhysicalDevice::memory_properties() const {
        return _memory_properties;
    }

    const VkPhysicalDeviceFeatures &PhysicalDevice::features() const {
        return _features;
    }

    const std::vector<VkQueueFamilyProperties> &
    PhysicalDevice::queue_families() const {
        return _queue_families;
    }

    const std::vector<VkExtensionProperties> &
    PhysicalDevice::extensions() const {
        return _extensions;
    }

    const std::vector<const char *> &
    PhysicalDevice::required_extensions() const {
        return _required_extensions;
    }

    const QueueFamily &PhysicalDevice::graphics_queues() const {
        return _graphics_queues;
    }

    const QueueFamily &PhysicalDevice::present_queues() const {
        return _present_queues;
    }

    const QueueFamily &PhysicalDevice::transfer_queues() const {
        return _transfer_queues;
    }

    const QueueFamily &PhysicalDevice::compute_queues() const {
        return _compute_queues;
    }

    int PhysicalDevice::score() const {
        // Minimum requirements
        if (!_features.fillModeNonSolid || !_features.sampleRateShading ||
            !_features.samplerAnisotropy || !_features.multiViewport ||
            !_graphics_queues.count || !_transfer_queues.count ||
            !_present_queues.count || !_compute_queues.count ||
            !supports_required_extensions()) {
            return 0;
        }

        // Prioritize discrete GPU
        int value = 0;
        switch (_properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            value += 1000;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            value += 500;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            value += 250;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            value += 100;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            value += 50;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
            break;
        }

        // Prioritize device with certain features
        value += _features.geometryShader * 1000;
        value += _features.tessellationShader * 1000;
        value += _features.wideLines * 500;

        // Prioritize device with higher limits
        value += _properties.limits.maxImageDimension2D;
        value += _properties.limits.maxDrawIndexedIndexValue;
        value += _properties.limits.maxMemoryAllocationCount;

        return value;
    }
} // namespace Dynamo::Graphics::Vulkan