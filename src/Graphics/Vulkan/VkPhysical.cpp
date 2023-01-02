#include "./VkPhysical.hpp"

namespace Dynamo::Graphics {
    VkPhysical::VkPhysical(vk::PhysicalDevice handle, vk::SurfaceKHR &surface) :
        _surface(surface) {
        _handle = handle;

        _properties = _handle.getProperties();
        _memory_properties = _handle.getMemoryProperties();
        _features = _handle.getFeatures();

        // Add the extensions required by all devices
        _extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

        // Enable portability subset extension if available
        const char *portability_ext = "VK_KHR_portability_subset";
        for (vk::ExtensionProperties &ext :
             _handle.enumerateDeviceExtensionProperties()) {
            if (!std::strcmp(ext.extensionName, portability_ext)) {
                _extensions.push_back(portability_ext);
            }
        }

        enumerate_command_queues();
        get_swapchain_options();
    }

    bool VkPhysical::is_complete() const {
        return _graphics_queue_properties.count > 0 &&
               _present_queue_properties.count > 0 &&
               _transfer_queue_properties.count > 0;
    }

    bool VkPhysical::is_supporting_extensions() const {
        auto available = _handle.enumerateDeviceExtensionProperties();
        std::set<std::string> required(_extensions.begin(), _extensions.end());

        for (vk::ExtensionProperties &extension : available) {
            required.erase(extension.extensionName);
        }
        return required.empty();
    }

    bool VkPhysical::is_supporting_swapchain() const {
        return !_swapchain_options.formats.empty() &&
               !_swapchain_options.present_modes.empty();
    }

    void VkPhysical::enumerate_command_queues() {
        auto families = _handle.getQueueFamilyProperties();
        unsigned index = 0;
        for (vk::QueueFamilyProperties &family : families) {
            // Dedicated presentation queue
            if (_handle.getSurfaceSupportKHR(index, _surface)) {
                _present_queue_properties.family_id = index;
                _present_queue_properties.count = family.queueCount;
            }

            // Dedicated graphics queue
            if (family.queueFlags & vk::QueueFlagBits::eGraphics) {
                _graphics_queue_properties.family_id = index;
                _graphics_queue_properties.count = family.queueCount;
            }

            // Dedicated transfer queue
            if (family.queueFlags & vk::QueueFlagBits::eTransfer) {
                _transfer_queue_properties.family_id = index;
                _transfer_queue_properties.count = family.queueCount;
            }

            if (is_complete()) {
                break;
            }
            index++;
        }

        // If the graphics and transfer queues are not available, use the
        // presentation queue
        if (_graphics_queue_properties.count == 0) {
            _graphics_queue_properties = _present_queue_properties;
        }
        if (_transfer_queue_properties.count == 0) {
            _transfer_queue_properties = _present_queue_properties;
        }
    }

    const vk::PhysicalDevice &VkPhysical::get_handle() const { return _handle; }

    const std::string VkPhysical::get_name() const {
        return _properties.deviceName;
    }

    const std::vector<const char *> &VkPhysical::get_extensions() const {
        return _extensions;
    }

    const SwapchainOptions &VkPhysical::get_swapchain_options() {
        _swapchain_options = {
            _handle.getSurfaceCapabilitiesKHR(_surface),
            _handle.getSurfaceFormatsKHR(_surface),
            _handle.getSurfacePresentModesKHR(_surface),
        };
        return _swapchain_options;
    }

    const QueueProperties &VkPhysical::get_graphics_queue_properties() const {
        return _graphics_queue_properties;
    }

    const QueueProperties &VkPhysical::get_present_queue_properties() const {
        return _present_queue_properties;
    }

    const QueueProperties &VkPhysical::get_transfer_queue_properties() const {
        return _transfer_queue_properties;
    }

    int VkPhysical::calculate_score() const {
        int score = 0;

        // Ensure all necessary features are present
        if (!is_complete() || !is_supporting_extensions() ||
            !is_supporting_swapchain() || !_features.samplerAnisotropy) {
            return 0;
        }

        // Dedicated GPU are prioritized
        vk::PhysicalDeviceType discrete = vk::PhysicalDeviceType::eDiscreteGpu;
        if (_properties.deviceType == discrete) {
            score += 1000;
        }

        // What is the largest image that can be stored in VRAM?
        score += _properties.limits.maxImageDimension2D;
        return score;
    }
} // namespace Dynamo::Graphics