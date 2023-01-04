#include "./PhysicalDevice.hpp"

namespace Dynamo::Graphics::Vulkan {
    PhysicalDevice::PhysicalDevice(vk::PhysicalDevice handle,
                                   vk::SurfaceKHR surface) {
        _handle = handle;
        _surface = surface;

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

    bool PhysicalDevice::is_complete() const {
        return _graphics_queue_properties.count > 0 &&
               _present_queue_properties.count > 0 &&
               _transfer_queue_properties.count > 0;
    }

    bool PhysicalDevice::is_supporting_extensions() const {
        auto available = _handle.enumerateDeviceExtensionProperties();
        std::unordered_set<std::string> required(_extensions.begin(),
                                                 _extensions.end());

        for (vk::ExtensionProperties &extension : available) {
            required.erase(extension.extensionName);
        }
        return required.empty();
    }

    bool PhysicalDevice::is_supporting_swapchain() const {
        return !_swapchain_options.formats.empty() &&
               !_swapchain_options.present_modes.empty();
    }

    void PhysicalDevice::enumerate_command_queues() {
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

    const vk::PhysicalDevice &PhysicalDevice::get_handle() const {
        return _handle;
    }

    const std::string PhysicalDevice::get_name() const {
        return _properties.deviceName;
    }

    const std::vector<const char *> &PhysicalDevice::get_extensions() const {
        return _extensions;
    }

    const SwapchainOptions &PhysicalDevice::get_swapchain_options() {
        _swapchain_options = {
            _handle.getSurfaceCapabilitiesKHR(_surface),
            _handle.getSurfaceFormatsKHR(_surface),
            _handle.getSurfacePresentModesKHR(_surface),
        };
        return _swapchain_options;
    }

    const QueueProperties &
    PhysicalDevice::get_queue_properties(QueueFamily family) const {
        switch (family) {
        case QueueFamily::Graphics:
            return _graphics_queue_properties;
            break;
        case QueueFamily::Transfer:
            return _transfer_queue_properties;
            break;
        case QueueFamily::Present:
        default:
            return _present_queue_properties;
            break;
        }
    }

    vk::FormatProperties
    PhysicalDevice::get_format_properties(vk::Format format) const {
        return _handle.getFormatProperties(format);
    }

    const vk::PhysicalDeviceMemoryProperties
    PhysicalDevice::get_memory_properties() const {
        return _memory_properties;
    }

    vk::SampleCountFlagBits PhysicalDevice::get_msaa_samples() const {
        vk::SampleCountFlags counts =
            _properties.limits.framebufferColorSampleCounts;

        // Get the maximum available sample count for improved visuals
        if (counts & vk::SampleCountFlagBits::e64)
            return vk::SampleCountFlagBits::e64;
        if (counts & vk::SampleCountFlagBits::e32)
            return vk::SampleCountFlagBits::e32;
        if (counts & vk::SampleCountFlagBits::e16)
            return vk::SampleCountFlagBits::e16;
        if (counts & vk::SampleCountFlagBits::e8)
            return vk::SampleCountFlagBits::e8;
        if (counts & vk::SampleCountFlagBits::e4)
            return vk::SampleCountFlagBits::e4;
        if (counts & vk::SampleCountFlagBits::e2)
            return vk::SampleCountFlagBits::e2;
        return vk::SampleCountFlagBits::e1;
    }

    vk::Format PhysicalDevice::get_depth_format() const {
        std::array<vk::Format, 3> query = {vk::Format::eD32Sfloat,
                                           vk::Format::eD32SfloatS8Uint,
                                           vk::Format::eD24UnormS8Uint};
        vk::ImageTiling tiling = vk::ImageTiling::eOptimal;
        vk::FormatFeatureFlags format_flags =
            vk::FormatFeatureFlagBits::eDepthStencilAttachment;

        // Find a format that satisfies the feature requirements
        for (vk::Format &format : query) {
            vk::FormatProperties properties = get_format_properties(format);
            if (tiling == vk::ImageTiling::eLinear &&
                (properties.linearTilingFeatures & format_flags) ==
                    format_flags) {
                return format;
            } else if (tiling == vk::ImageTiling::eOptimal &&
                       (properties.optimalTilingFeatures & format_flags) ==
                           format_flags) {
                return format;
            }
        }
        Log::error("Vulkan failed to find a suitable pixel format for the "
                   "depth buffer.");
        return query[0];
    }

    int PhysicalDevice::calculate_score() const {
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
} // namespace Dynamo::Graphics::Vulkan