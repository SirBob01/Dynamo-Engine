#pragma once
#define GLFW_INCLUDE_VULKAN
#ifndef NDEBUG
#define VK_DEBUG true
#else
#define VK_DEBUG false
#endif

#include <memory>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../Renderer.hpp"
#include "./Debugger.hpp"
#include "./Device.hpp"
#include "./Image.hpp"
#include "./ImageAllocator.hpp"
#include "./PhysicalDevice.hpp"
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
        std::unique_ptr<ImageAllocator> _image_allocator;
        std::unique_ptr<Swapchain> _swapchain;

        std::unique_ptr<UserImage> _depth_image;
        std::unique_ptr<ImageView> _depth_view;

        std::unique_ptr<UserImage> _color_image;
        std::unique_ptr<ImageView> _color_view;

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
         * @brief Create the memory allocators
         *
         */
        void create_memory_allocators();

        /**
         * @brief Create the swapchain
         *
         */
        void create_swapchain();

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
    };
} // namespace Dynamo::Graphics::Vulkan