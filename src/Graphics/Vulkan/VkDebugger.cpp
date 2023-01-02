#include "./VkDebugger.hpp"

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

namespace Dynamo::Graphics {
    VkDebugger::VkDebugger(vk::Instance &instance) {
        load_proxies(instance);

        // Create the debug messenger
        vk::DebugUtilsMessengerCreateInfoEXT create_info;
        create_info.messageSeverity =
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
        create_info.messageType =
            vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
        create_info.pfnUserCallback = &message_callback;

        _messenger = instance.createDebugUtilsMessengerEXTUnique(create_info);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VkDebugger::message_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        VkDebugUtilsMessengerCallbackDataEXT const *data,
        void *user_data) {

        Log::warn("--- VkDebugger Message ---");
        Log::warn("Message name: {}", data->pMessageIdName);
        Log::warn("Message Id: {}", data->messageIdNumber);

        // Terminate on error
        if (static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(severity) ==
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError) {
            Log::error(data->pMessage);
        } else {
            Log::warn(data->pMessage);
        }
        return VK_FALSE;
    }

    void VkDebugger::load_proxies(vk::Instance &instance) {
        vk_create_debugger_dispatch =
            reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
                instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        vk_destroy_debugger_dispatch =
            reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
                instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
    }
} // namespace Dynamo::Graphics