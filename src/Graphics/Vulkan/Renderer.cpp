#include "Renderer.hpp"

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(Display &display, const std::string asset_directory) :
        Dynamo::Graphics::Renderer(display, asset_directory) {
        enumerate_extensions();
        create_instance();
        create_surface();
        create_device();
        create_objects();
        create_swapchain();
    }

    Renderer::~Renderer() {
        // Wait for all queues to finish processing commands
        _device->wait();
    }

    void Renderer::enumerate_extensions() {
        // Get supported extensions from GLFW
        u32 count;
        const i8 **extensions = glfwGetRequiredInstanceExtensions(&count);
        for (i32 i = 0; i < count; i++) {
            _extensions.push_back(extensions[i]);
        }

#ifdef DYN_DEBUG
        // Enable validation layers on debug mode
        _validation_layers.push_back("VK_LAYER_KHRONOS_validation");
        _extensions.push_back("VK_EXT_debug_utils");
        Log::info("--- Vulkan Extensions ---");
        for (const i8 *extension : _extensions) {
            Log::info("* {}", extension);
        }
        Log::info("");
#endif
    }

    b8 Renderer::is_supporting_layers() {
        auto layer_properties = vk::enumerateInstanceLayerProperties();
        for (const i8 *requested : _validation_layers) {
            for (vk::LayerProperties &available : layer_properties) {
                if (!std::strcmp(requested, available.layerName)) {
                    return true;
                }
            }
        }
        return false;
    }

    void Renderer::create_instance() {
#ifdef DYN_DEBUG
        if (!is_supporting_layers()) {
            Log::error("Requested Vulkan validation layers unavailable.");
        }
#endif

        // Setup the application and Vulkan instance
        vk::ApplicationInfo app_info;
        app_info.pApplicationName = _display.get().get_title().c_str();
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

#ifdef DYN_DEBUG
        // Include more validation layers on debug mode
        std::vector<vk::ValidationFeatureEnableEXT> layer_extensions = {
            vk::ValidationFeatureEnableEXT::eBestPractices,
            vk::ValidationFeatureEnableEXT::eDebugPrintf,
        };
        vk::ValidationFeaturesEXT features_info(layer_extensions);
        instance_info.pNext = &features_info;
#endif

        // Create the instance and debugger
        _instance = vk::createInstanceUnique(instance_info);
        _debugger = std::make_unique<Debugger>(*_instance);
    }

    void Renderer::create_surface() {
        VkSurfaceKHR tmp_surface;
        vk::Result result = static_cast<vk::Result>(
            glfwCreateWindowSurface(_instance.get(),
                                    _display.get().get_window(),
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

#ifdef DYN_DEBUG
        Log::info("--- Vulkan Physical Devices ---");
        for (PhysicalDevice &device : physical_devices) {
            if (device.get_handle() == _device->get_physical().get_handle()) {
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
#endif
    }

    void Renderer::create_objects() {
        // Pools
        _memory_pool = std::make_unique<MemoryPool>(*_device);
        _descriptor_pool = std::make_unique<DescriptorPool>(*_device);
        _graphics_command_pool =
            std::make_unique<CommandPool>(*_device, QueueFamily::Graphics);
        _transfer_command_pool =
            std::make_unique<CommandPool>(*_device, QueueFamily::Transfer);

        // Texture sampler
        _sampler = std::make_unique<Sampler>(*_device, 3);

        // Create synchronizers
        _signal_image_ready.clear();
        _signal_render_done.clear();
        _fences.clear();

        for (i32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            _signal_image_ready.push_back(
                std::make_unique<Semaphore>(*_device));
            _signal_render_done.push_back(
                std::make_unique<Semaphore>(*_device));
            _fences.push_back(std::make_unique<Fence>(*_device));
        }

        // Command queues
        _graphics_queue = _device->get_queue(QueueFamily::Graphics);
        _transfer_queue = _device->get_queue(QueueFamily::Transfer);
        _present_queue = _device->get_queue(QueueFamily::Present);

        // Staging buffer for memory copies
        vk::MemoryPropertyFlags staging_memory_properties =
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent |
            vk::MemoryPropertyFlagBits::eHostCached;
        vk::BufferUsageFlags staging_buffer_usage =
            vk::BufferUsageFlagBits::eTransferSrc;
        _staging_buffer = std::make_unique<Buffer>(*_device,
                                                   *_memory_pool,
                                                   *_transfer_command_pool,
                                                   _transfer_queue,
                                                   staging_buffer_usage,
                                                   staging_memory_properties);

        // Object mesh buffer
        vk::MemoryPropertyFlags object_memory_properties =
            vk::MemoryPropertyFlagBits::eDeviceLocal;
        vk::BufferUsageFlags object_buffer_usage =
            vk::BufferUsageFlagBits::eIndexBuffer |
            vk::BufferUsageFlagBits::eVertexBuffer;
        _object_buffer = std::make_unique<Buffer>(*_device,
                                                  *_memory_pool,
                                                  *_transfer_command_pool,
                                                  _transfer_queue,
                                                  object_buffer_usage,
                                                  object_memory_properties);

        // Uniform buffer
        vk::MemoryPropertyFlags uniform_memory_properties =
            vk::MemoryPropertyFlagBits::eHostVisible |
            vk::MemoryPropertyFlagBits::eHostCoherent |
            vk::MemoryPropertyFlagBits::eHostCached;
        vk::BufferUsageFlags uniform_buffer_usage =
            vk::BufferUsageFlagBits::eUniformBuffer;
        _uniform_buffer = std::make_unique<Buffer>(*_device,
                                                   *_memory_pool,
                                                   *_transfer_command_pool,
                                                   _transfer_queue,
                                                   uniform_buffer_usage,
                                                   uniform_memory_properties);
    }

    void Renderer::create_swapchain() {
        // Wait for all GPU processes to finish
        _device->wait();

        // Destroy existing swapchain and material system before
        // initializing
        _material_system.reset();
        _draw_set.reset();
        _swapchain.reset();

        _swapchain = std::make_unique<Swapchain>(*_device,
                                                 *_memory_pool,
                                                 _display.get(),
                                                 _surface.get());
        _material_system = std::make_unique<MaterialSystem>(*_device,
                                                            *_swapchain,
                                                            *_descriptor_pool,
                                                            *_uniform_buffer);
        _draw_set = std::make_unique<DrawSet>(*_swapchain,
                                              *_graphics_command_pool,
                                              *_object_buffer,
                                              _graphics_queue);
    }

    AssetCache<Mesh> &Renderer::get_mesh_assets() { return *_mesh_assets; }

    AssetCache<Dynamo::Graphics::Texture> &Renderer::get_texture_assets() {
        return *_texture_assets;
    }

    AssetCache<Dynamo::Graphics::Shader> &Renderer::get_shader_assets() {
        return *_shader_assets;
    }

    Dynamo::Graphics::MaterialSystem &Renderer::get_material_system() {
        return *_material_system;
    }

    std::unique_ptr<Dynamo::Graphics::MeshInstance>
    Renderer::upload_mesh(Mesh &mesh) {
        return std::make_unique<MeshInstance>(mesh,
                                              *_object_buffer,
                                              *_staging_buffer);
    }

    void Renderer::set_layers(std::vector<Dynamo::Graphics::Layer> &layers) {
        // TODO
    }

    void Renderer::draw(Dynamo::Graphics::MeshInstance &mesh_instance,
                        Dynamo::Graphics::Material &material,
                        std::vector<Mat4> &transforms,
                        u32 layer) {
        // Downcast API agnostic objects
        Material &vk_material = *dynamic_cast<Material *>(&material);
        MeshInstance &vk_mesh_instance =
            *dynamic_cast<MeshInstance *>(&mesh_instance);
        _draw_set->add(vk_mesh_instance, vk_material, transforms);
    }

    void Renderer::clear(Color color) { _draw_set->clear(color); }

    void Renderer::refresh() {
        // Record the command buffers
        _draw_set->record();

        // Grab the next available swapchain image presentation
        // target
        std::optional<u32> acquired =
            _swapchain->get_presentation_image(*_signal_image_ready[_frame]);
        if (!acquired.has_value()) {
            create_swapchain();
            return;
        }
        u32 image_index = acquired.value();

        // Wait for the fence to finish and reset before proceeding
        _fences[_frame]->wait();
        _fences[_frame]->reset();

        // Submit commands to the graphics queue for rendering to
        // that image
        vk::PipelineStageFlags wait_stage =
            vk::PipelineStageFlagBits::eColorAttachmentOutput;
        vk::SubmitInfo submit_info;
        submit_info.pWaitDstStageMask = &wait_stage;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores =
            &_signal_image_ready[_frame]->get_handle();
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores =
            &_signal_render_done[_frame]->get_handle();
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers =
            &_draw_set->get_command_buffer(image_index);

        // Render to the target attachment and signal the current
        // fence
        _graphics_queue.submit(submit_info, _fences[_frame]->get_handle());

        // Present rendered image to the display
        vk::PresentInfoKHR present_info;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores =
            &_signal_render_done[_frame]->get_handle();
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &_swapchain->get_handle();
        present_info.pImageIndices = &image_index;

        try {
            vk::Result result = _present_queue.presentKHR(present_info);

            // Sometimes, it does not throw an error
            if (result != vk::Result::eSuccess) {
                create_swapchain();
            }
        } catch (vk::OutOfDateKHRError e) {
            create_swapchain();
        }

        // Advance the frame
        _frame = (_frame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
} // namespace Dynamo::Graphics::Vulkan