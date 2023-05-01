#include "Renderer.hpp"

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(Display &display) : _display(display) {
        enumerate_extensions();
        create_instance();
        create_surface();

        create_device();
        create_pools();

        create_swapchain();
        create_synchronizers();

        create_depth_buffer();
        create_color_buffer();

        create_framebuffers();
        create_pipeline();
        create_command_buffers();

        create_buffers();

        clear(Color(0, 0, 0));
        _depth_clear.depthStencil.depth = 1;
        _depth_clear.depthStencil.stencil = 0;
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
        _sampler = std::make_unique<Sampler>(*_device, 3);

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

    void Renderer::create_pools() {
        _memory_pool = std::make_unique<MemoryPool>(*_device);
        _descriptor_pool = std::make_unique<DescriptorPool>(*_device);
        _graphics_command_pool =
            std::make_unique<CommandPool>(*_device, QueueFamily::Graphics);
        _transfer_command_pool =
            std::make_unique<CommandPool>(*_device, QueueFamily::Transfer);
    }

    void Renderer::create_swapchain() {
        _swapchain = std::make_unique<Swapchain>(*_device, _display, *_surface);
    }

    void Renderer::create_synchronizers() {
        _signal_image_ready.clear();
        _signal_render_done.clear();
        _fences.clear();

        for (i32 i = 0; i < _max_frames_processing; i++) {
            _signal_image_ready.push_back(
                std::make_unique<Semaphore>(*_device));
            _signal_render_done.push_back(
                std::make_unique<Semaphore>(*_device));
            _fences.push_back(std::make_unique<Fence>(*_device));
        }
    }

    void Renderer::create_depth_buffer() {
        vk::Extent2D extent = _swapchain->get_extent();
        _depth_image = std::make_unique<UserImage>(
            *_device,
            *_memory_pool,
            extent.width,
            extent.height,
            1,
            1,
            1,
            _device->get_physical().get_depth_format(),
            vk::ImageType::e2D,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            _device->get_physical().get_msaa_samples());
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
            *_memory_pool,
            extent.width,
            extent.height,
            1,
            1,
            1,
            _swapchain->get_format().format,
            vk::ImageType::e2D,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment,
            _device->get_physical().get_msaa_samples());
        _color_view =
            std::make_unique<ImageView>(*_color_image,
                                        vk::ImageViewType::e2D,
                                        vk::ImageAspectFlagBits::eColor,
                                        1);
    }

    void Renderer::create_framebuffers() {
        _renderpass = std::make_unique<RenderPass>(*_device, *_swapchain);
        vk::Extent2D extent = _swapchain->get_extent();

        _framebuffers.clear();
        for (const std::unique_ptr<ImageView> &view : _swapchain->get_views()) {
            _framebuffers.push_back(std::make_unique<Framebuffer>(*_device,
                                                                  extent,
                                                                  *_renderpass,
                                                                  *_color_view,
                                                                  *_depth_view,
                                                                  *view));
        }
    }

    void Renderer::create_pipeline() {
        // Default shaders
        _vertex_shader =
            std::make_unique<ShaderModule>(*_device,
                                           "vert.spv",
                                           vk::ShaderStageFlagBits::eVertex);
        _fragment_shader =
            std::make_unique<ShaderModule>(*_device,
                                           "frag.spv",
                                           vk::ShaderStageFlagBits::eFragment);

        // Create the pipeline and allocate the descriptors
        ShaderList shaders = {
            *_vertex_shader,
            *_fragment_shader,
        };
        _pipeline_layout = std::make_unique<PipelineLayout>(*_device, shaders);
        _pipeline =
            std::make_unique<Pipeline>(*_device,
                                       *_renderpass,
                                       *_swapchain,
                                       *_pipeline_layout,
                                       vk::PrimitiveTopology::eTriangleList,
                                       vk::PolygonMode::eFill);
        _descriptor_pool->allocate(*_pipeline_layout, *_swapchain);
    }

    void Renderer::create_command_buffers() {
        // Define the command queues
        _graphics_queue = _device->get_queue(QueueFamily::Graphics);
        _transfer_queue = _device->get_queue(QueueFamily::Transfer);
        _present_queue = _device->get_queue(QueueFamily::Present);

        // Create command buffers for each family
        _graphics_command_buffers =
            _graphics_command_pool->allocate(vk::CommandBufferLevel::ePrimary,
                                             _framebuffers.size());
    }

    void Renderer::create_buffers() {
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

    void Renderer::record_commands() {
        _graphics_queue.waitIdle();
        _graphics_command_pool->reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        vk::Rect2D render_area;
        render_area.offset.x = 0;
        render_area.offset.y = 0;
        render_area.extent = _swapchain->get_extent();

        std::array<vk::ClearValue, 2> clear_values = {
            _color_clear,
            _depth_clear,
        };

        vk::CommandBufferBeginInfo begin_info;
        for (u32 i = 0; i < _framebuffers.size(); i++) {
            // Start recording
            _graphics_command_buffers[i]->begin(begin_info);

            // Start the render pass
            vk::RenderPassBeginInfo render_begin_info;
            render_begin_info.renderPass = _renderpass->get_handle();
            render_begin_info.framebuffer = _framebuffers[i]->get_handle();
            render_begin_info.renderArea = render_area;
            render_begin_info.clearValueCount = clear_values.size();
            render_begin_info.pClearValues = clear_values.data();

            _graphics_command_buffers[i]->beginRenderPass(
                render_begin_info,
                vk::SubpassContents::eInline);

            // Bind the command buffer to the graphics pipeline
            _graphics_command_buffers[i]->bindPipeline(
                vk::PipelineBindPoint::eGraphics,
                _pipeline->get_handle());

            // Stop recording
            _graphics_command_buffers[i]->endRenderPass();
            _graphics_command_buffers[i]->end();
        }
    }

    void Renderer::reset_swapchain() {
        // Wait for all GPU processes to finish
        _device->wait();

        // Reset the views before image to prevent invalid references (segfault)
        // TODO: Best to use shared_ptr or some custom refcount implementation
        _depth_view.reset();
        _color_view.reset();

        // Free the swapchain
        _swapchain.reset();

        // Recreate objects
        create_swapchain();
        create_synchronizers();

        create_depth_buffer();
        create_color_buffer();

        create_framebuffers();
        create_pipeline();
        create_command_buffers();
    }

    Texture Renderer::create_texture(std::string filename) {
        i32 width, height, channels;
        stbi_uc *raw_data = stbi_load(filename.c_str(),
                                      &width,
                                      &height,
                                      &channels,
                                      STBI_rgb_alpha);
        std::vector<u8> pixels(width * height * STBI_rgb_alpha);
        std::copy(raw_data, raw_data + pixels.size(), pixels.data());
        stbi_image_free(raw_data);

        return Texture(pixels.data(),
                       width,
                       height,
                       *_device,
                       *_memory_pool,
                       *_staging_buffer,
                       *_graphics_command_pool,
                       _graphics_queue);
    }

    void Renderer::clear(Color color) {
        _color_clear.color.setFloat32(color.to_array());
    }

    void Renderer::refresh() {
        // Record the command buffers
        record_commands();

        // Grab the next available swapchain image presentation target
        std::optional<u32> acquired =
            _swapchain->get_presentation_image(*_signal_image_ready[_frame]);
        if (!acquired.has_value()) {
            reset_swapchain();
            return;
        }
        u32 image_index = acquired.value();

        // Wait for the fence to finish and reset before proceeding
        _fences[_frame]->wait();
        _fences[_frame]->reset();

        // Submit commands to the graphics queue for rendering to that image
        vk::PipelineStageFlags wait_stage =
            vk::PipelineStageFlagBits::eColorAttachmentOutput;
        vk::SubmitInfo submit_info;
        submit_info.pWaitDstStageMask = &wait_stage;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores =
            &_signal_image_ready[_frame]->get_handle();
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers =
            &_graphics_command_buffers[image_index].get();
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores =
            &_signal_render_done[_frame]->get_handle();

        // Render to the target attachment and signal the current fence
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
                reset_swapchain();
            }
        } catch (vk::OutOfDateKHRError e) {
            reset_swapchain();
        }

        // Advance the frame
        _frame = (_frame + 1) % _max_frames_processing;
    }
} // namespace Dynamo::Graphics::Vulkan