#pragma once

#include <vulkan/vulkan_core.h>

#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Build the debug messenger.
     *
     * @param instance
     * @return VkDebugUtilsMessengerEXT
     */
    VkDebugUtilsMessengerEXT
    VkDebugUtilsMessengerEXT_build(VkInstance instance);

    /**
     * @brief Debug messenger callback.
     *
     * @param severity
     * @param type
     * @param data
     * @param user_data
     * @return VKAPI_ATTR
     */
    VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugUtilsMessengerEXT_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        VkDebugUtilsMessengerCallbackDataEXT const *data,
        void *user_data);

} // namespace Dynamo::Graphics::Vulkan