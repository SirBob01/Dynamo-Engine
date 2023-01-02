#include "VkRenderer.hpp"

namespace Dynamo::Graphics {
    VkRenderer::VkRenderer(Display &display) : Renderer(display) {
        create_instance();
    }

    VkRenderer::~VkRenderer() {}

    bool VkRenderer::is_supporting_layers() {
        auto layer_properties = vk::enumerateInstanceLayerProperties();
        for (const char *requested : _validation_layers) {
            for (vk::LayerProperties &available : layer_properties) {
                if (!std::strcmp(requested, available.layerName)) {
                    return true;
                }
            }
        }
        return false;
    }

    void VkRenderer::create_instance() {
        if (!is_supporting_layers() && VKDEBUG) {
            Log::error("Requested Vulkan validation layers unavailable.");
        }

        // Setup the application and Vk instance
        vk::ApplicationInfo app_info;
        app_info.pApplicationName = _display.get_title().c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Dynamo Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_MAKE_VERSION(1, 2, 0);

        vk::InstanceCreateInfo instance_info;
        instance_info.pApplicationInfo = &app_info;

        // Load validation layers
        instance_info.enabledLayerCount = _validation_layers.size();
        instance_info.ppEnabledLayerNames = _validation_layers.data();

        // Load extensions
        instance_info.enabledExtensionCount = _extensions.size();
        instance_info.ppEnabledExtensionNames = _extensions.data();

        // Include more validation layers on debug mode
        std::vector<vk::ValidationFeatureEnableEXT> layer_extensions = {
            vk::ValidationFeatureEnableEXT::eBestPractices,
            vk::ValidationFeatureEnableEXT::eDebugPrintf,
        };
        vk::ValidationFeaturesEXT features_info(layer_extensions);
        if (VKDEBUG) {
            instance_info.pNext = &features_info;
        }

        // Create the instance and, if necessary, the debugger
        _instance = vk::createInstanceUnique(instance_info);
        if (VKDEBUG) {
            _debugger = std::make_unique<VkDebugger>(*_instance);
        }
    }
} // namespace Dynamo::Graphics