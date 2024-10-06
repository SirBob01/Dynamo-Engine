#include <Graphics/Renderer.hpp>
#include <algorithm>

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(const Display &display) {
        _instance = std::make_unique<Instance>(display);
#ifdef DYN_DEBUG
        _debugger = std::make_unique<Debugger>(*_instance);
#endif
        _surface = std::make_unique<Surface>(*_instance, display);
        _device = std::make_unique<Device>(find_physical_device());
    }

    PhysicalDevice &Renderer::find_physical_device() {
        unsigned count = 0;
        vkEnumeratePhysicalDevices(_instance->handle(), &count, nullptr);

        std::vector<VkPhysicalDevice> handles(count);
        vkEnumeratePhysicalDevices(_instance->handle(), &count, handles.data());

        for (VkPhysicalDevice handle : handles) {
            _physical_devices.emplace_back(handle, *_surface);
        }

        if (count == 0) {
            Log::error("No Vulkan-compatible physical devices available.");
        }

        // Find the highest scoring device
        PhysicalDevice &best = *std::max_element(
            _physical_devices.begin(),
            _physical_devices.end(),
            [](auto &a, auto &b) { return a.score() < b.score(); });

        // Make sure the best device we have supports everything we need
        if (best.score() == 0) {
            Log::error("No Vulkan::PhysicalDevice meets minimum requirements.");
        }

        // Log device information
        Log::info("Vulkan-compatible physical devices:");
        for (const PhysicalDevice &device : _physical_devices) {
            if (device.handle() == best.handle()) {
                Log::info(">>> {} (Vendor Id: {} | Score: {})",
                          device.properties().deviceName,
                          device.properties().vendorID,
                          device.score());

            } else {
                Log::info("* {} (Vendor Id: {} | Score: {})",
                          device.properties().deviceName,
                          device.properties().vendorID,
                          device.score());
            }
        }
        Log::info("");

        // Log device extensions
        Log::info("Vulkan {} extensions:", best.properties().deviceName);
        for (const VkExtensionProperties &extension : best.extensions()) {
            bool enabled = false;
            for (const char *ext_name : best.required_extensions()) {
                if (!std::strcmp(extension.extensionName, ext_name)) {
                    enabled = true;
                    break;
                }
            }
            if (enabled) {
                Log::info(">>> {}", extension.extensionName);
            } else {
                Log::info("* {}", extension.extensionName);
            }
        }
        Log::info("");

        // Log device queue families
        Log::info("Vulkan graphics queues (Family Index: {} | Count: {})",
                  best.graphics_queues().index,
                  best.graphics_queues().count);
        Log::info("Vulkan present queues (Family Index: {} | Count: {})",
                  best.present_queues().index,
                  best.present_queues().count);
        Log::info("Vulkan transfer queues (Family Index: {} | Count: {})",
                  best.transfer_queues().index,
                  best.transfer_queues().count);
        Log::info("Vulkan compute queues (Family Index: {} | Count: {})",
                  best.compute_queues().index,
                  best.compute_queues().count);
        Log::info("");

        return best;
    }
} // namespace Dynamo::Graphics::Vulkan