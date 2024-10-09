#include <Graphics/Vulkan/Debugger.hpp>
#include <Graphics/Vulkan/Utils.hpp>

static PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebuggerDispatch;
static PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebuggerDispatch;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *create_info,
    const VkAllocationCallbacks *allocator,
    VkDebugUtilsMessengerEXT *messenger) {
    return vkCreateDebuggerDispatch(instance,
                                    create_info,
                                    allocator,
                                    messenger);
}

VKAPI_ATTR void VKAPI_CALL
vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                VkDebugUtilsMessengerEXT messenger,
                                VkAllocationCallbacks const *allocator) {
    vkDestroyDebuggerDispatch(instance, messenger, allocator);
}

namespace Dynamo::Graphics::Vulkan {
    VkDebugUtilsMessengerEXT
    VkDebugUtilsMessengerEXT_build(VkInstance instance) {
        vkCreateDebuggerDispatch =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(instance,
                                      "vkCreateDebugUtilsMessengerEXT"));
        vkDestroyDebuggerDispatch =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                vkGetInstanceProcAddr(instance,
                                      "vkDestroyDebugUtilsMessengerEXT"));

        // Create the debug messenger
        VkDebugUtilsMessengerCreateInfoEXT debugger_info = {};
        debugger_info.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugger_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        debugger_info.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugger_info.pfnUserCallback =
            &VkDebugUtilsMessengerEXT_message_callback;

        VkDebugUtilsMessengerEXT debugger;
        VkResult_log("Create Debugger",
                     vkCreateDebugUtilsMessengerEXT(instance,
                                                    &debugger_info,
                                                    nullptr,
                                                    &debugger));
        return debugger;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugUtilsMessengerEXT_message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
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