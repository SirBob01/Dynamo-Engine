#pragma once
#define GLFW_INCLUDE_VULKAN
#ifndef NDEBUG
#define VK_DEBUG true
#else
#define VK_DEBUG false
#endif

#include <memory>
#include <set>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../Renderer.hpp"
#include "./VkDebugger.hpp"
#include "./VkPhysical.hpp"
#include "./VkSwapchain.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Renderer powered by the Vulkan API
     *
     */
    class VkRenderer : public Renderer {
        /**
         * @brief Vulkan instance
         *
         */
        vk::UniqueInstance _instance;

        /**
         * @brief Surface
         *
         */
        vk::UniqueSurfaceKHR _surface;

        /**
         * @brief Logical device
         *
         */
        vk::UniqueDevice _logical;

        /**
         * @brief Command queues
         *
         */
        vk::Queue _graphics_queue;
        vk::Queue _present_queue;
        vk::Queue _transfer_queue;

        /**
         * @brief Rendering swapchain
         *
         */
        std::unique_ptr<VkSwapchain> _swapchain;

        /**
         * @brief Physical device
         *
         */
        std::unique_ptr<VkPhysical> _physical;

        /**
         * @brief Debugger
         *
         */
        std::unique_ptr<VkDebugger> _debugger;

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
         * @brief Select the appropriate GPU
         *
         */
        void create_physical_device();

        /**
         * @brief Create a logical device given the selected physical device
         *
         */
        void create_logical_device();

        /**
         * @brief Create the swapchain
         *
         */
        void create_swapchain();

      public:
        /**
         * @brief Construct a new VkRenderer object
         *
         * @param display
         */
        VkRenderer(Display &display);

        /**
         * @brief Destroy the VkRenderer object
         *
         */
        ~VkRenderer();
    };
} // namespace Dynamo::Graphics