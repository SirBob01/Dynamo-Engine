#pragma once
#define GLFW_INCLUDE_VULKAN

#include <memory>
#include <unordered_set>
#include <vector>

#include <stb_image.h>
#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "../../Types.hpp"
#include "../Renderer.hpp"
#include "./Core/Buffer.hpp"
#include "./Core/CommandPool.hpp"
#include "./Core/Debugger.hpp"
#include "./Core/DescriptorPool.hpp"
#include "./Core/Device.hpp"
#include "./Core/Fence.hpp"
#include "./Core/Framebuffer.hpp"
#include "./Core/Image.hpp"
#include "./Core/MemoryPool.hpp"
#include "./Core/PhysicalDevice.hpp"
#include "./Core/Pipeline.hpp"
#include "./Core/PipelineLayout.hpp"
#include "./Core/RenderPass.hpp"
#include "./Core/Sampler.hpp"
#include "./Core/Semaphore.hpp"
#include "./Core/ShaderModule.hpp"
#include "./Core/Swapchain.hpp"
#include "./Core/Texture.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Renderer powered by the Vulkan API
     *
     */
    class Renderer {
        std::reference_wrapper<Display> _display;

        vk::UniqueInstance _instance;
        vk::UniqueSurfaceKHR _surface;

        std::unique_ptr<Device> _device;
        std::unique_ptr<Sampler> _sampler;

        std::unique_ptr<Swapchain> _swapchain;

        // Pools
        std::unique_ptr<MemoryPool> _memory_pool;
        std::unique_ptr<DescriptorPool> _descriptor_pool;
        std::unique_ptr<CommandPool> _graphics_command_pool;
        std::unique_ptr<CommandPool> _transfer_command_pool;

        // Framebuffers
        std::unique_ptr<UserImage> _depth_image;
        std::unique_ptr<ImageView> _depth_view;

        std::unique_ptr<UserImage> _color_image;
        std::unique_ptr<ImageView> _color_view;

        std::unique_ptr<RenderPass> _renderpass;
        std::vector<std::unique_ptr<Framebuffer>> _framebuffers;

        // Graphics pipeline
        std::unique_ptr<ShaderModule> _vertex_shader;
        std::unique_ptr<ShaderModule> _fragment_shader;
        std::unique_ptr<PipelineLayout> _pipeline_layout;
        std::unique_ptr<Pipeline> _pipeline;

        // Command buffers and queues
        vk::Queue _graphics_queue;
        vk::Queue _transfer_queue;
        vk::Queue _present_queue;

        std::vector<vk::UniqueCommandBuffer> _graphics_command_buffers;

        // Buffers
        std::unique_ptr<Buffer> _staging_buffer;
        std::unique_ptr<Buffer> _object_buffer;
        std::unique_ptr<Buffer> _uniform_buffer;

        // Synchronizers
        // Semaphores synchronize the graphic and present commands
        std::vector<std::unique_ptr<Semaphore>> _signal_image_ready;
        std::vector<std::unique_ptr<Semaphore>> _signal_render_done;

        // Fences synchronize the GPU and CPU processes
        std::vector<std::unique_ptr<Fence>> _fences;

        // Clear values
        vk::ClearValue _color_clear;
        vk::ClearValue _depth_clear;

        // Frame counters
        u32 _frame = 0;
        u32 _max_frames_processing = 3;

        /**
         * @brief Debugger
         *
         */
        std::unique_ptr<Debugger> _debugger;

        /**
         * @brief List of supported extensions
         *
         */
        std::vector<const i8 *> _extensions;

        /**
         * @brief List of enabled validation layers for debugging
         *
         */
        std::vector<const i8 *> _validation_layers;

        /**
         * @brief Enumerate all the supported extensions
         *
         */
        void enumerate_extensions();

        /**
         * @brief Check if the system supports the enabled validation layers
         *
         * @return true
         * @return false
         */
        b8 is_supporting_layers();

        /**
         * @brief Create a new vk::UniqueInstance
         *
         */
        void create_instance();

        /**
         * @brief Attach the GLFW window to a vk::Surface
         *
         */
        void create_surface();

        /**
         * @brief Select a suitable hardware device and create the logical
         * device
         *
         */
        void create_device();

        /**
         * @brief Create the synchronizer objects
         *
         */
        void create_synchronizers();

        /**
         * @brief Create the swapchain
         *
         */
        void create_swapchain();

        /**
         * @brief Create the image sampler
         *
         */
        void create_sampler();

        /**
         * @brief Create the allocators
         *
         */
        void create_pools();

        /**
         * @brief Create the depth buffer
         *
         */
        void create_depth_buffer();

        /**
         * @brief Create the color buffer
         *
         */
        void create_color_buffer();

        /**
         * @brief Create the framebuffers for each swapchain view
         *
         */
        void create_framebuffers();

        /**
         * @brief Create the graphics pipeline
         *
         */
        void create_pipeline();

        /**
         * @brief Create a command buffers
         *
         */
        void create_command_buffers();

        /**
         * @brief Create the buffers for reading and writing data to the GPU
         *
         */
        void create_buffers();

        /**
         * @brief Record the draw commands
         *
         */
        void record_commands();

        /**
         * @brief Reset the swapchain
         *
         */
        void reset_swapchain();

      public:
        /**
         * @brief Construct a new Renderer object
         *
         * @param display
         */
        Renderer(Display &display);

        /**
         * @brief Destroy the Renderer object
         *
         */
        ~Renderer();

        /**
         * @brief Create a texture from an image file
         *
         * @param filename
         * @return Texture
         */
        Texture create_texture(std::string filename);

        /**
         * @brief Clear the display with a color
         *
         */
        void clear(Color color);

        /**
         * @brief Update the renderer and present to the display surface
         *
         */
        void refresh();
    };
} // namespace Dynamo::Graphics::Vulkan