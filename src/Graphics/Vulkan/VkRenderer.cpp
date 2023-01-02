#include "VkRenderer.hpp"

namespace Dynamo::Graphics {
    VkRenderer::VkRenderer(Display &display) : Renderer(display) {
        enumerate_extensions();
        create_instance();
        create_surface();

        create_physical_device();
        create_logical_device();
    }

    VkRenderer::~VkRenderer() {}

    void VkRenderer::enumerate_extensions() {
        // Get supported extensions from GLFW
        unsigned count;
        const char **extensions = glfwGetRequiredInstanceExtensions(&count);
        for (int i = 0; i < count; i++) {
            _extensions.push_back(extensions[i]);
        }

        // Enable validation layers if on debug mode
        if (VK_DEBUG) {
            _validation_layers.push_back("VK_LAYER_KHRONOS_validation");
            _extensions.push_back("VK_EXT_debug_utils");
            Log::info("--- Vulkan Extensions ---");
            for (const char *extension : _extensions) {
                Log::info("* {}", extension);
            }
            Log::info("");
        }
    }

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
        if (!is_supporting_layers() && VK_DEBUG) {
            Log::error("Requested Vulkan validation layers unavailable.");
        }

        // Setup the application and Vk instance
        vk::ApplicationInfo app_info;
        app_info.pApplicationName = _display.get_title().c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Dynamo Engine";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_MAKE_VERSION(1, 3, 0);

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
        if (VK_DEBUG) {
            instance_info.pNext = &features_info;
        }

        // Create the instance and, if necessary, the debugger
        _instance = vk::createInstanceUnique(instance_info);
        if (VK_DEBUG) {
            _debugger = std::make_unique<VkDebugger>(*_instance);
        }
    }

    void VkRenderer::create_surface() {
        VkSurfaceKHR tmp_surface;
        vk::Result result = static_cast<vk::Result>(
            glfwCreateWindowSurface(_instance.get(),
                                    _display.get_window(),
                                    nullptr,
                                    &tmp_surface));
        _surface = vk::UniqueSurfaceKHR(tmp_surface, _instance.get());
        if (result != vk::Result::eSuccess) {
            Log::error("Failed to create a Vulkan surface: {}",
                       vk::to_string(result));
        }
    }

    void VkRenderer::create_physical_device() {
        std::vector<VkPhysical> devices;
        for (vk::PhysicalDevice vkd : _instance->enumeratePhysicalDevices()) {
            VkPhysical device(vkd, _surface.get());
            if (device.calculate_score() > 0) {
                devices.emplace_back(device);
            }
        }
        if (devices.empty()) {
            Log::error("Failed to identify a Vulkan physical device.");
        }

        // Select the device with the highest score
        _physical = std::make_unique<VkPhysical>(*std::max_element(
            devices.begin(),
            devices.end(),
            [&](const VkPhysical &a, const VkPhysical &b) {
                return a.calculate_score() < b.calculate_score();
            }));

        if (VK_DEBUG) {
            Log::info("--- Vulkan Physical Devices ---");
            for (VkPhysical &device : devices) {
                if (device.get_handle() == _physical->get_handle()) {
                    Log::info("* {} | Score: {} [Selected]",
                              device.get_name(),
                              device.calculate_score());
                } else {
                    Log::info("* {} | Score: {}",
                              device.get_name(),
                              device.calculate_score());
                }
            }
            Log::info("");
        }
    }

    void VkRenderer::create_logical_device() {
        vk::PhysicalDevice &physical_handle = _physical->get_handle();
        QueueProperties graphics_queue_properties =
            _physical->get_graphics_queue_properties();
        QueueProperties present_queue_properties =
            _physical->get_present_queue_properties();
        QueueProperties transfer_queue_properties =
            _physical->get_transfer_queue_properties();

        // Get all unique queue families
        auto cmp = [](QueueProperties a, QueueProperties b) {
            return a.family_id != b.family_id;
        };
        std::set<QueueProperties, decltype(cmp)> unique_families(cmp);
        unique_families.insert(graphics_queue_properties);
        unique_families.insert(present_queue_properties);
        unique_families.insert(transfer_queue_properties);

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
        const auto &extensions = _physical->get_extensions();

        vk::DeviceCreateInfo device_info;
        device_info.queueCreateInfoCount = queue_infos.size();
        device_info.pQueueCreateInfos = queue_infos.data();
        device_info.enabledExtensionCount = extensions.size();
        device_info.ppEnabledExtensionNames = extensions.data();
        device_info.pEnabledFeatures = &device_features;
        device_info.pNext = &descriptor_indexing;

        _logical = physical_handle.createDeviceUnique(device_info);

        // Grab device queue handles
        _graphics_queue =
            _logical->getQueue(graphics_queue_properties.family_id, 0);
        _present_queue =
            _logical->getQueue(present_queue_properties.family_id, 0);
        _transfer_queue =
            _logical->getQueue(transfer_queue_properties.family_id, 0);

        // Create the image memory allocator (TODO)
        // image_memory_ =
        //     std::make_unique<ImageMemoryAllocator>(logical_.get(),
        //     *physical_);
    }
} // namespace Dynamo::Graphics