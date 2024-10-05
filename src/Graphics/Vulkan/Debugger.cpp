#include <Graphics/Vulkan/Debugger.hpp>

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *messenger) {
    return vk_create_debugger_dispatch(instance,
                                       create_info,
                                       allocator,
                                       messenger);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                VkDebugUtilsMessengerEXT messenger,
                                VkAllocationCallbacks const *allocator) {
    vk_destroy_debugger_dispatch(instance, messenger, allocator);
}

namespace Dynamo::Graphics::Vulkan {
    Debugger::Debugger(Instance &instance) : _instance(instance) {
        vk_create_debugger_dispatch =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(_instance.handle(),
                                      "vkCreateDebugUtilsMessengerEXT"));
        vk_destroy_debugger_dispatch =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(_instance.handle(),
                                      "vkDestroyDebugUtilsMessengerEXT"));

        // Create the debug messenger
        VkDebugUtilsMessengerCreateInfoEXT debug_utils_info = {};
        debug_utils_info.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_utils_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        debug_utils_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_utils_info.pfnUserCallback = &message_callback;

        VkResult result = vkCreateDebugUtilsMessengerEXT(_instance.handle(),
                                                         &debug_utils_info,
                                                         nullptr,
                                                         &_handle);
        if (result != VK_SUCCESS) {
            Log::error("Could not create Vulkan::Debugger.");
        }
    }

    Debugger::~Debugger() {
        vk_destroy_debugger_dispatch(_instance.handle(), _handle, nullptr);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL
    Debugger::message_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                               VkDebugUtilsMessageTypeFlagsEXT type,
                               VkDebugUtilsMessengerCallbackDataEXT const *data,
                               void *user_data) {
        Log::warn("--- Vulkan::Debugger Message ---");
        Log::warn("Message name: {}", data->pMessageIdName);
        Log::warn("Message Id: {}", data->messageIdNumber);

        // Terminate on error
        if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            Log::error(data->pMessage);
        } else {
            Log::warn(data->pMessage);
        }
        Log::warn("");
        return VK_FALSE;
    }
} // namespace Dynamo::Graphics::Vulkan