#pragma once
#define GLFW_INCLUDE_VULKAN
#ifndef NDEBUG
#define VKDEBUG true
#else
#define VKDEBUG false
#endif

#include <memory>

#include <vulkan/vulkan.hpp>

#include "../Renderer.hpp"
#include "./VkDebugger.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Renderer powered by the Vulkan API
     *
     */
    class VkRenderer : public Renderer {
        vk::UniqueInstance _instance;
        std::unique_ptr<VkDebugger> _debugger;

        /**
         * @brief Extensions and validation layers
         *
         */
        std::vector<const char *> _extensions = {
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        };
        std::vector<const char *> _validation_layers = {
            "VK_LAYER_KHRONOS_validation",
        };

        /**
         * @brief Check if the system supports the required validation layers
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