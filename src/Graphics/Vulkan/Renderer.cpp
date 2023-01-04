#include "Renderer.hpp"

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(Display &display) : Dynamo::Graphics::Renderer(display) {
        enumerate_extensions();
        create_instance();
        create_surface();

        create_device();
        create_memory_allocators();
        create_swapchain();

        create_depth_buffer();
        create_color_buffer();
    }

    Renderer::~Renderer() {}

    void Renderer::enumerate_extensions() {
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

    bool Renderer::is_supporting_layers() {
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

    void Renderer::create_instance() {
        if (!is_supporting_layers() && VK_DEBUG) {
            Log::error("Requested Vulkan validation layers unavailable.");
        }

        // Setup the application and Vulkan instance
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
            _debugger = std::make_unique<Debugger>(*_instance);
        }
    }

    void Renderer::create_surface() {
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

    void Renderer::create_device() {
        // Get the list of physical devices
        std::vector<PhysicalDevice> physical_devices;
        for (vk::PhysicalDevice vkd : _instance->enumeratePhysicalDevices()) {
            PhysicalDevice physical(vkd, _surface.get());
            if (physical.calculate_score() > 0) {
                physical_devices.emplace_back(physical);
            }
        }
        if (physical_devices.empty()) {
            Log::error("Failed to identify a Vulkan physical device.");
        }

        // Select the device with the highest score
        _device = std::make_unique<Device>(*std::max_element(
            physical_devices.begin(),
            physical_devices.end(),
            [&](const PhysicalDevice &a, const PhysicalDevice &b) {
                return a.calculate_score() < b.calculate_score();
            }));

        if (VK_DEBUG) {
            Log::info("--- Vulkan Physical Devices ---");
            for (PhysicalDevice &device : physical_devices) {
                if (device.get_handle() ==
                    _device->get_physical().get_handle()) {
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

    void Renderer::create_memory_allocators() {
        _image_allocator = std::make_unique<ImageAllocator>(*_device);
    }

    void Renderer::create_swapchain() {
        _swapchain = std::make_unique<Swapchain>(_display, *_device, *_surface);
    }

    void Renderer::create_depth_buffer() {
        vk::Extent2D extent = _swapchain->get_extent();
        _depth_image = std::make_unique<UserImage>(
            *_device,
            extent.width,
            extent.height,
            1,
            1,
            1,
            _device->get_physical().get_depth_format(),
            vk::ImageType::e2D,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment);
        _image_allocator->allocate(*_depth_image);
        _depth_view =
            std::make_unique<ImageView>(*_depth_image,
                                        vk::ImageViewType::e2D,
                                        vk::ImageAspectFlagBits::eDepth,
                                        1);
    }

    void Renderer::create_color_buffer() {
        vk::Extent2D extent = _swapchain->get_extent();
        _color_image = std::make_unique<UserImage>(
            *_device,
            extent.width,
            extent.height,
            1,
            1,
            1,
            _swapchain->get_format().format,
            vk::ImageType::e2D,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment);
        _image_allocator->allocate(*_color_image);
        _color_view =
            std::make_unique<ImageView>(*_color_image,
                                        vk::ImageViewType::e2D,
                                        vk::ImageAspectFlagBits::eColor,
                                        1);
    }
} // namespace Dynamo::Graphics::Vulkan