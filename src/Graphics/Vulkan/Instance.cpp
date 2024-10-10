#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    static std::array<const char *, 1> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation",
    };

    void check_validation_layers() {
        unsigned count;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        std::vector<VkLayerProperties> layer_list(count);
        vkEnumerateInstanceLayerProperties(&count, layer_list.data());

        // Check if all requested layers are available
        for (const char *layer_name : VALIDATION_LAYERS) {
            bool found = false;
            for (VkLayerProperties &properties : layer_list) {
                if (!std::strcmp(properties.layerName, layer_name)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                Log::error("Requested layer {} for Vulkan::Instance not supported.", layer_name);
            }
        }
    }

    VkInstance VkInstance_build(const Display &display) {
        std::vector<const char *> extensions = display.get_vulkan_extensions();

        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = display.get_title().c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Dynamo";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo instance_info = {};
        instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_info.pApplicationInfo = &app_info;

// Enable validation layers if debug mode is on
#ifdef DYN_DEBUG
        check_validation_layers();
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        instance_info.enabledLayerCount = VALIDATION_LAYERS.size();
        instance_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#else
        instance_info.enabledLayerCount = 0;
        instance_info.ppEnabledLayerNames = nullptr;
#endif

        instance_info.enabledExtensionCount = extensions.size();
        instance_info.ppEnabledExtensionNames = extensions.data();

        Log::info("Required Vulkan extensions:");
        for (const char *ext : extensions) {
            Log::info("* {}", ext);
        }
        Log::info("");

        VkInstance instance;
        VkResult_log("Create Instance", vkCreateInstance(&instance_info, nullptr, &instance));
        return instance;
    }
} // namespace Dynamo::Graphics::Vulkan