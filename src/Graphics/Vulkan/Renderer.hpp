#pragma once
#define GLFW_INCLUDE_VULKAN

#include <memory>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "../Renderer.hpp"
#include "./Allocator.hpp"
#include "./CommandAllocator.hpp"
#include "./Debugger.hpp"
#include "./DescriptorAllocator.hpp"
#include "./Device.hpp"
#include "./Fence.hpp"
#include "./Framebuffer.hpp"
#include "./Image.hpp"
#include "./PhysicalDevice.hpp"
#include "./Pipeline.hpp"
#include "./PipelineLayout.hpp"
#include "./RenderPass.hpp"
#include "./Sampler.hpp"
#include "./Semaphore.hpp"
#include "./ShaderModule.hpp"
#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Renderer powered by the Vulkan API
     *
     */
    class Renderer : public Dynamo::Graphics::Renderer {
        vk::UniqueInstance _instance;
        vk::UniqueSurfaceKHR _surface;

        std::unique_ptr<Device> _device;
        std::unique_ptr<Sampler> _sampler;

        std::unique_ptr<Swapchain> _swapchain;

        // Allocators
        std::unique_ptr<Allocator> _allocator;
        std::unique_ptr<DescriptorAllocator> _descriptor_allocator;
        std::unique_ptr<CommandAllocator> _graphics_command_allocator;
        std::unique_ptr<CommandAllocator> _transfer_command_allocator;

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
        std::vector<vk::UniqueCommandBuffer> _transfer_command_buffers;

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
        unsigned _frame = 0;
        unsigned _max_frames_processing = 3;

        /**
         * @brief Debugger
         *
         */
        std::unique_ptr<Debugger> _debugger;

        /**
         * @brief List of supported extensions
         *
         */
        std::vector<const char *> _extensions;

        /**
         * @brief List of enabled validation layers for debugging
         *
         */
        std::vector<const char *> _validation_layers;

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
        bool is_supporting_layers();

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
        void create_allocators();

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
         * @brief Update the renderer and present to the display surface
         *
         */
        void refresh() override;

        /**
         * @brief Clear the display with a color
         *
         */
        void clear(Color color) override;
    };
} // namespace Dynamo::Graphics::Vulkan