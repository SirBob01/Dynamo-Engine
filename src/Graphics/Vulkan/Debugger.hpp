#pragma once

#include <vulkan/vulkan.h>

#include <Graphics/Vulkan/Instance.hpp>
#include <Utils/Log.hpp>

static PFN_vkCreateDebugUtilsMessengerEXT vk_create_debugger_dispatch;
static PFN_vkDestroyDebugUtilsMessengerEXT vk_destroy_debugger_dispatch;

/**
 * @brief API override for instancing the debug messenger
 *
 * @param instance
 * @param create_info
 * @param allocator
 * @param messenger
 * @return VkResult
 */
VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *messenger);

/**
 * @brief API override for destroying the debug messenger
 *
 * @param instance
 * @param messenger
 * @param allocator
 */
VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                VkDebugUtilsMessengerEXT messenger,
                                VkAllocationCallbacks const *allocator);

namespace Dynamo::Graphics::Vulkan {
    class Debugger {
        Instance &_instance;
        VkDebugUtilsMessengerEXT _handle;

        /**
         * @brief Message callback for error logging.
         *
         * @param severity
         * @param type
         * @param data
         * @param user_data
         * @return VKAPI_ATTR
         */
        static VKAPI_ATTR VkBool32 VKAPI_CALL
        message_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                         VkDebugUtilsMessageTypeFlagsEXT type,
                         VkDebugUtilsMessengerCallbackDataEXT const *data,
                         void *user_data);

      public:
        Debugger(Instance &instance);
        ~Debugger();
    };
} // namespace Dynamo::Graphics::Vulkan