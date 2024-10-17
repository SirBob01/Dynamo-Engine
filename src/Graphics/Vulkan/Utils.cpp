#include <Graphics/Vulkan/Utils.hpp>
#include <vulkan/vulkan_core.h>

static PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebuggerDispatch;
static PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebuggerDispatch;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                              const VkDebugUtilsMessengerCreateInfoEXT *create_info,
                                                              const VkAllocationCallbacks *allocator,
                                                              VkDebugUtilsMessengerEXT *messenger) {
    return vkCreateDebuggerDispatch(instance, create_info, allocator, messenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                           VkDebugUtilsMessengerEXT messenger,
                                                           VkAllocationCallbacks const *allocator) {
    vkDestroyDebuggerDispatch(instance, messenger, allocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL
VkDebugUtilsMessengerEXT_message_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                          VkDebugUtilsMessageTypeFlagsEXT type,
                                          VkDebugUtilsMessengerCallbackDataEXT const *data,
                                          void *user_data) {
    Dynamo::Log::warn("--- Vulkan::Debugger Message ---");
    Dynamo::Log::warn("Message name: {}", data->pMessageIdName);
    Dynamo::Log::warn("Message Id: {}", data->messageIdNumber);

    // Terminate on error
    if (severity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        Dynamo::Log::error(data->pMessage);
    } else {
        Dynamo::Log::warn(data->pMessage);
    }
    Dynamo::Log::warn("");
    return VK_FALSE;
}

namespace Dynamo::Graphics::Vulkan {
    bool RenderPassSettings::operator==(const RenderPassSettings &other) const {
        return color_format == other.color_format && depth_format == other.depth_format &&
               clear_color == other.clear_color && clear_depth == other.clear_depth;
    }

    bool FramebufferSettings::operator==(const FramebufferSettings &other) const {
        return view == other.view && extent.width == other.extent.width && extent.height == other.extent.height &&
               pass == other.pass;
    }

    bool GraphicsPipelineSettings::operator==(const GraphicsPipelineSettings &other) const {
        return topology == other.topology && polygon_mode == other.polygon_mode && cull_mode == other.cull_mode &&
               vertex.handle == other.vertex.handle && fragment.handle == other.fragment.handle &&
               layout == other.layout && renderpass == other.renderpass;
    }

    unsigned VkFormat_size(VkFormat format) {
        switch (format) {
        case VK_FORMAT_UNDEFINED:
            return 0;
        case VK_FORMAT_R4G4_UNORM_PACK8:
            return 1;
        case VK_FORMAT_R4G4B4A4_UNORM_PACK16:
            return 2;
        case VK_FORMAT_B4G4R4A4_UNORM_PACK16:
            return 2;
        case VK_FORMAT_R5G6B5_UNORM_PACK16:
            return 2;
        case VK_FORMAT_B5G6R5_UNORM_PACK16:
            return 2;
        case VK_FORMAT_R5G5B5A1_UNORM_PACK16:
            return 2;
        case VK_FORMAT_B5G5R5A1_UNORM_PACK16:
            return 2;
        case VK_FORMAT_A1R5G5B5_UNORM_PACK16:
            return 2;
        case VK_FORMAT_R8_UNORM:
            return 1;
        case VK_FORMAT_R8_SNORM:
            return 1;
        case VK_FORMAT_R8_USCALED:
            return 1;
        case VK_FORMAT_R8_SSCALED:
            return 1;
        case VK_FORMAT_R8_UINT:
            return 1;
        case VK_FORMAT_R8_SINT:
            return 1;
        case VK_FORMAT_R8_SRGB:
            return 1;
        case VK_FORMAT_R8G8_UNORM:
            return 2;
        case VK_FORMAT_R8G8_SNORM:
            return 2;
        case VK_FORMAT_R8G8_USCALED:
            return 2;
        case VK_FORMAT_R8G8_SSCALED:
            return 2;
        case VK_FORMAT_R8G8_UINT:
            return 2;
        case VK_FORMAT_R8G8_SINT:
            return 2;
        case VK_FORMAT_R8G8_SRGB:
            return 2;
        case VK_FORMAT_R8G8B8_UNORM:
            return 3;
        case VK_FORMAT_R8G8B8_SNORM:
            return 3;
        case VK_FORMAT_R8G8B8_USCALED:
            return 3;
        case VK_FORMAT_R8G8B8_SSCALED:
            return 3;
        case VK_FORMAT_R8G8B8_UINT:
            return 3;
        case VK_FORMAT_R8G8B8_SINT:
            return 3;
        case VK_FORMAT_R8G8B8_SRGB:
            return 3;
        case VK_FORMAT_B8G8R8_UNORM:
            return 3;
        case VK_FORMAT_B8G8R8_SNORM:
            return 3;
        case VK_FORMAT_B8G8R8_USCALED:
            return 3;
        case VK_FORMAT_B8G8R8_SSCALED:
            return 3;
        case VK_FORMAT_B8G8R8_UINT:
            return 3;
        case VK_FORMAT_B8G8R8_SINT:
            return 3;
        case VK_FORMAT_B8G8R8_SRGB:
            return 3;
        case VK_FORMAT_R8G8B8A8_UNORM:
            return 4;
        case VK_FORMAT_R8G8B8A8_SNORM:
            return 4;
        case VK_FORMAT_R8G8B8A8_USCALED:
            return 4;
        case VK_FORMAT_R8G8B8A8_SSCALED:
            return 4;
        case VK_FORMAT_R8G8B8A8_UINT:
            return 4;
        case VK_FORMAT_R8G8B8A8_SINT:
            return 4;
        case VK_FORMAT_R8G8B8A8_SRGB:
            return 4;
        case VK_FORMAT_B8G8R8A8_UNORM:
            return 4;
        case VK_FORMAT_B8G8R8A8_SNORM:
            return 4;
        case VK_FORMAT_B8G8R8A8_USCALED:
            return 4;
        case VK_FORMAT_B8G8R8A8_SSCALED:
            return 4;
        case VK_FORMAT_B8G8R8A8_UINT:
            return 4;
        case VK_FORMAT_B8G8R8A8_SINT:
            return 4;
        case VK_FORMAT_B8G8R8A8_SRGB:
            return 4;
        case VK_FORMAT_A8B8G8R8_UNORM_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SNORM_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_USCALED_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SSCALED_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_UINT_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SINT_PACK32:
            return 4;
        case VK_FORMAT_A8B8G8R8_SRGB_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_UNORM_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_SNORM_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_USCALED_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_SSCALED_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_UINT_PACK32:
            return 4;
        case VK_FORMAT_A2R10G10B10_SINT_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_SNORM_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_USCALED_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_SSCALED_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_UINT_PACK32:
            return 4;
        case VK_FORMAT_A2B10G10R10_SINT_PACK32:
            return 4;
        case VK_FORMAT_R16_UNORM:
            return 2;
        case VK_FORMAT_R16_SNORM:
            return 2;
        case VK_FORMAT_R16_USCALED:
            return 2;
        case VK_FORMAT_R16_SSCALED:
            return 2;
        case VK_FORMAT_R16_UINT:
            return 2;
        case VK_FORMAT_R16_SINT:
            return 2;
        case VK_FORMAT_R16_SFLOAT:
            return 2;
        case VK_FORMAT_R16G16_UNORM:
            return 4;
        case VK_FORMAT_R16G16_SNORM:
            return 4;
        case VK_FORMAT_R16G16_USCALED:
            return 4;
        case VK_FORMAT_R16G16_SSCALED:
            return 4;
        case VK_FORMAT_R16G16_UINT:
            return 4;
        case VK_FORMAT_R16G16_SINT:
            return 4;
        case VK_FORMAT_R16G16_SFLOAT:
            return 4;
        case VK_FORMAT_R16G16B16_UNORM:
            return 6;
        case VK_FORMAT_R16G16B16_SNORM:
            return 6;
        case VK_FORMAT_R16G16B16_USCALED:
            return 6;
        case VK_FORMAT_R16G16B16_SSCALED:
            return 6;
        case VK_FORMAT_R16G16B16_UINT:
            return 6;
        case VK_FORMAT_R16G16B16_SINT:
            return 6;
        case VK_FORMAT_R16G16B16_SFLOAT:
            return 6;
        case VK_FORMAT_R16G16B16A16_UNORM:
            return 8;
        case VK_FORMAT_R16G16B16A16_SNORM:
            return 8;
        case VK_FORMAT_R16G16B16A16_USCALED:
            return 8;
        case VK_FORMAT_R16G16B16A16_SSCALED:
            return 8;
        case VK_FORMAT_R16G16B16A16_UINT:
            return 8;
        case VK_FORMAT_R16G16B16A16_SINT:
            return 8;
        case VK_FORMAT_R16G16B16A16_SFLOAT:
            return 8;
        case VK_FORMAT_R32_UINT:
            return 4;
        case VK_FORMAT_R32_SINT:
            return 4;
        case VK_FORMAT_R32_SFLOAT:
            return 4;
        case VK_FORMAT_R32G32_UINT:
            return 8;
        case VK_FORMAT_R32G32_SINT:
            return 8;
        case VK_FORMAT_R32G32_SFLOAT:
            return 8;
        case VK_FORMAT_R32G32B32_UINT:
            return 12;
        case VK_FORMAT_R32G32B32_SINT:
            return 12;
        case VK_FORMAT_R32G32B32_SFLOAT:
            return 12;
        case VK_FORMAT_R32G32B32A32_UINT:
            return 16;
        case VK_FORMAT_R32G32B32A32_SINT:
            return 16;
        case VK_FORMAT_R32G32B32A32_SFLOAT:
            return 16;
        case VK_FORMAT_R64_UINT:
            return 8;
        case VK_FORMAT_R64_SINT:
            return 8;
        case VK_FORMAT_R64_SFLOAT:
            return 8;
        case VK_FORMAT_R64G64_UINT:
            return 16;
        case VK_FORMAT_R64G64_SINT:
            return 16;
        case VK_FORMAT_R64G64_SFLOAT:
            return 16;
        case VK_FORMAT_R64G64B64_UINT:
            return 24;
        case VK_FORMAT_R64G64B64_SINT:
            return 24;
        case VK_FORMAT_R64G64B64_SFLOAT:
            return 24;
        case VK_FORMAT_R64G64B64A64_UINT:
            return 32;
        case VK_FORMAT_R64G64B64A64_SINT:
            return 32;
        case VK_FORMAT_R64G64B64A64_SFLOAT:
            return 32;
        case VK_FORMAT_B10G11R11_UFLOAT_PACK32:
            return 4;
        case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32:
            return 4;
        default:
            Log::error("Invalid VkFormat size.");
            return 0;
        }
    }

    const char *VkPhysicalDeviceType_string(VkPhysicalDeviceType type) {
        switch (type) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "VK_PHYSICAL_DEVICE_TYPE_CPU";
        case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM:
            return "VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM";
        default:
            return "";
        }
    }

    const char *VkShaderStageFlagBits_string(VkShaderStageFlagBits stage) {
        switch (stage) {
        case VK_SHADER_STAGE_VERTEX_BIT:
            return "VK_SHADER_STAGE_VERTEX_BIT";
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
            return "VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT";
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
            return "VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT";
        case VK_SHADER_STAGE_GEOMETRY_BIT:
            return "VK_SHADER_STAGE_GEOMETRY_BIT";
        case VK_SHADER_STAGE_FRAGMENT_BIT:
            return "VK_SHADER_STAGE_FRAGMENT_BIT";
        case VK_SHADER_STAGE_COMPUTE_BIT:
            return "VK_SHADER_STAGE_COMPUTE_BIT";
        case VK_SHADER_STAGE_ALL_GRAPHICS:
            return "VK_SHADER_STAGE_ALL_GRAPHICS";
        case VK_SHADER_STAGE_ALL:
            return "VK_SHADER_STAGE_ALL";
        case VK_SHADER_STAGE_RAYGEN_BIT_KHR:
            return "VK_SHADER_STAGE_RAYGEN_BIT_KHR";
        case VK_SHADER_STAGE_ANY_HIT_BIT_KHR:
            return "VK_SHADER_STAGE_ANY_HIT_BIT_KHR";
        case VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR:
            return "VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR";
        case VK_SHADER_STAGE_MISS_BIT_KHR:
            return "VK_SHADER_STAGE_MISS_BIT_KHR";
        case VK_SHADER_STAGE_INTERSECTION_BIT_KHR:
            return "VK_SHADER_STAGE_INTERSECTION_BIT_KHR";
        case VK_SHADER_STAGE_CALLABLE_BIT_KHR:
            return "VK_SHADER_STAGE_CALLABLE_BIT_KHR";
        case VK_SHADER_STAGE_TASK_BIT_NV:
            return "VK_SHADER_STAGE_TASK_BIT_NV";
        case VK_SHADER_STAGE_MESH_BIT_NV:
            return "VK_SHADER_STAGE_MESH_BIT_NV";
        case VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI:
            return "VK_SHADER_STAGE_SUBPASS_SHADING_BIT_HUAWEI";
        default:
            return "";
        }
    }

    const char *VkResult_string(VkResult result) {
        switch (result) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";
        case VK_ERROR_OUT_OF_POOL_MEMORY:
            return "VK_ERROR_OUT_OF_POOL_MEMORY";
        case VK_ERROR_INVALID_EXTERNAL_HANDLE:
            return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
        case VK_ERROR_FRAGMENTATION:
            return "VK_ERROR_FRAGMENTATION";
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
            return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
        case VK_ERROR_SURFACE_LOST_KHR:
            return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
        case VK_SUBOPTIMAL_KHR:
            return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
            return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "VK_ERROR_VALIDATION_FAILED_EXT";
        case VK_ERROR_INVALID_SHADER_NV:
            return "VK_ERROR_INVALID_SHADER_NV";
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:
            return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
        case VK_ERROR_NOT_PERMITTED_EXT:
            return "VK_ERROR_NOT_PERMITTED_EXT";
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
            return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
        case VK_THREAD_IDLE_KHR:
            return "VK_THREAD_IDLE_KHR";
        case VK_THREAD_DONE_KHR:
            return "VK_THREAD_DONE_KHR";
        case VK_OPERATION_DEFERRED_KHR:
            return "VK_OPERATION_DEFERRED_KHR";
        case VK_OPERATION_NOT_DEFERRED_KHR:
            return "VK_OPERATION_NOT_DEFERRED_KHR";
        case VK_PIPELINE_COMPILE_REQUIRED_EXT:
            return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
        case VK_RESULT_MAX_ENUM:
            return "VK_RESULT_MAX_ENUM";
        default:
            return "";
        }
    }

    void VkResult_log(const std::string &op_message, VkResult result) {
        if (result != VK_SUCCESS) {
            Log::error("Graphics::Vulkan {}: {}", op_message, VkResult_string(result));
        }
    }

    VkInstance VkInstance_create(const Display &display) {
        std::vector<const char *> extensions = display.get_vulkan_extensions();

        VkApplicationInfo app_info = {};
        app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        app_info.pApplicationName = display.get_title().c_str();
        app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.pEngineName = "Dynamo";
        app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        app_info.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo instance_info = {};
        instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instance_info.pApplicationInfo = &app_info;

// Enable validation layers if debug mode is on
#ifdef DYN_DEBUG
        const char *layer = "VK_LAYER_KHRONOS_validation";
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        instance_info.enabledLayerCount = 1;
        instance_info.ppEnabledLayerNames = &layer;
#else
        instance_info.enabledLayerCount = 0;
        instance_info.ppEnabledLayerNames = nullptr;
#endif

        instance_info.enabledExtensionCount = extensions.size();
        instance_info.ppEnabledExtensionNames = extensions.data();

        Log::info("Required Vulkan extensions:");
        for (const char *ext : extensions) {
            Log::info("* {}", ext);
        }
        Log::info("");

        VkInstance instance;
        VkResult_log("Create Instance", vkCreateInstance(&instance_info, nullptr, &instance));
        return instance;
    }

    VkDebugUtilsMessengerEXT VkDebugUtilsMessengerEXT_create(VkInstance instance) {
        vkCreateDebuggerDispatch = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
        vkDestroyDebuggerDispatch = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));

        // Create the debug messenger
        VkDebugUtilsMessengerCreateInfoEXT debugger_info = {};
        debugger_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugger_info.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
        debugger_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugger_info.pfnUserCallback = &VkDebugUtilsMessengerEXT_message_callback;

        VkDebugUtilsMessengerEXT debugger;
        VkResult_log("Create Debugger", vkCreateDebugUtilsMessengerEXT(instance, &debugger_info, nullptr, &debugger));
        return debugger;
    }

    VkDevice VkDevice_create(const PhysicalDevice &physical) {
        // Enable physical device features
        VkPhysicalDeviceFeatures device_features = {};
        device_features.samplerAnisotropy = true;
        device_features.sampleRateShading = true;
        device_features.fillModeNonSolid = true;
        device_features.multiViewport = true;

        // Enable descriptor indexing features
        VkPhysicalDeviceDescriptorIndexingFeatures descriptor_indexing = {};
        descriptor_indexing.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
        descriptor_indexing.descriptorBindingPartiallyBound = true;
        descriptor_indexing.runtimeDescriptorArray = true;
        descriptor_indexing.descriptorBindingVariableDescriptorCount = true;

        std::vector<QueueFamilyRef> queue_families = physical.unique_queue_families();
        std::vector<const char *> extensions = physical.required_extensions();

        std::vector<VkDeviceQueueCreateInfo> queue_infos;
        for (const QueueFamily &family : queue_families) {
            VkDeviceQueueCreateInfo queue_info = {};
            queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_info.queueCount = family.count;
            queue_info.queueFamilyIndex = family.index;
            queue_info.pQueuePriorities = family.priorities.data();
            queue_infos.push_back(queue_info);
        }

        VkDeviceCreateInfo device_info = {};
        device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        device_info.queueCreateInfoCount = queue_infos.size();
        device_info.pQueueCreateInfos = queue_infos.data();
        device_info.enabledExtensionCount = extensions.size();
        device_info.ppEnabledExtensionNames = extensions.data();
        device_info.pEnabledFeatures = &device_features;
        device_info.pNext = &descriptor_indexing;

        VkDevice device;
        VkResult_log("Create Device", vkCreateDevice(physical.handle, &device_info, nullptr, &device));
        return device;
    }

    VkDeviceMemory VkDeviceMemory_allocate(VkDevice device, unsigned type_index, unsigned size) {
        VkMemoryAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc_info.memoryTypeIndex = type_index;
        alloc_info.allocationSize = size;

        VkDeviceMemory memory;
        VkResult_log("Allocate Memory", vkAllocateMemory(device, &alloc_info, nullptr, &memory));
        return memory;
    }

    VkBuffer VkBuffer_create(VkDevice device,
                             VkBufferUsageFlags usage,
                             unsigned size,
                             const QueueFamily *queue_families,
                             unsigned queue_family_count) {
        std::vector<unsigned> family_indices;
        for (unsigned i = 0; i < queue_family_count; i++) {
            family_indices.push_back(queue_families[i].index);
        }

        VkBufferCreateInfo buffer_info = {};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.usage = usage;
        buffer_info.size = size;
        buffer_info.queueFamilyIndexCount = family_indices.size();
        buffer_info.pQueueFamilyIndices = family_indices.data();

        if (family_indices.size() > 1) {
            buffer_info.sharingMode = VK_SHARING_MODE_CONCURRENT;
        } else {
            buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        VkBuffer buffer;
        VkResult_log("Create Buffer", vkCreateBuffer(device, &buffer_info, nullptr, &buffer));
        return buffer;
    }

    VkImageView VkImageView_create(VkDevice device, VkImage image, ImageViewSettings settings) {
        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = image;
        view_info.format = settings.format;
        view_info.viewType = settings.type;
        view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        view_info.subresourceRange.aspectMask = settings.aspect_mask;
        view_info.subresourceRange.baseMipLevel = settings.mip_base;
        view_info.subresourceRange.levelCount = settings.mip_count;
        view_info.subresourceRange.baseArrayLayer = settings.layer_base;
        view_info.subresourceRange.layerCount = settings.layer_count;

        VkImageView view;
        VkResult_log("Create ImageView", vkCreateImageView(device, &view_info, nullptr, &view));
        return view;
    }

    VkRenderPass VkRenderPass_create(VkDevice device, const RenderPassSettings &settings) {
        // Color buffer
        VkAttachmentDescription color = {};
        color.format = settings.color_format;
        color.loadOp = settings.clear_color ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
        color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        color.samples = VK_SAMPLE_COUNT_1_BIT; // TODO

        std::array<VkAttachmentDescription, 1> attachments = {color};

        VkAttachmentReference color_ref = {};
        color_ref.attachment = 0;
        color_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &color_ref;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderpass_info = {};
        renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderpass_info.attachmentCount = attachments.size();
        renderpass_info.pAttachments = attachments.data();
        renderpass_info.subpassCount = 1;
        renderpass_info.pSubpasses = &subpass;
        renderpass_info.dependencyCount = 1;
        renderpass_info.pDependencies = &dependency;

        VkRenderPass renderpass;
        VkResult_log("Create Render Pass", vkCreateRenderPass(device, &renderpass_info, nullptr, &renderpass));
        return renderpass;
    }

    VkPipelineLayout VkPipelineLayout_create(VkDevice device) {
        VkPipelineLayoutCreateInfo layout_info = {};
        layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layout_info.setLayoutCount = 0;
        layout_info.pSetLayouts = nullptr;
        layout_info.pushConstantRangeCount = 0;
        layout_info.pPushConstantRanges = nullptr;

        VkPipelineLayout layout;
        VkResult_log("Create Pipeline Layout", vkCreatePipelineLayout(device, &layout_info, nullptr, &layout));
        return layout;
    }

    VkShaderModule VkShaderModule_create(VkDevice device, const std::vector<uint32_t> &bytecode) {
        VkShaderModuleCreateInfo shader_info = {};
        shader_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_info.pCode = bytecode.data();
        shader_info.codeSize = bytecode.size() * sizeof(uint32_t);

        VkShaderModule shader;
        VkResult_log("Create Shader Module", vkCreateShaderModule(device, &shader_info, nullptr, &shader));
        return shader;
    }

    VkPipeline VkPipeline_create(VkDevice device,
                                 VkPipelineCache cache,
                                 VkRenderPass pass,
                                 const GraphicsPipelineSettings &settings) {
        VkGraphicsPipelineCreateInfo pipeline_info = {};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        // Dynamic pipeline states that need to be set during command recording
        std::array<VkDynamicState, 4> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_BLEND_CONSTANTS,
            VK_DYNAMIC_STATE_DEPTH_BOUNDS,
        };
        VkPipelineDynamicStateCreateInfo dynamic = {};
        dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic.dynamicStateCount = dynamic_states.size();
        dynamic.pDynamicStates = dynamic_states.data();
        pipeline_info.pDynamicState = &dynamic;

        // Programmable shader stages
        std::array<VkPipelineShaderStageCreateInfo, 2> stages;

        VkPipelineShaderStageCreateInfo vertex_stage_info = {};
        vertex_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_stage_info.module = settings.vertex.handle;
        vertex_stage_info.pName = "main";
        stages[0] = vertex_stage_info;

        VkPipelineShaderStageCreateInfo fragment_stage_info = {};
        fragment_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_stage_info.module = settings.fragment.handle;
        fragment_stage_info.pName = "main";
        stages[1] = fragment_stage_info;

        pipeline_info.stageCount = stages.size();
        pipeline_info.pStages = stages.data();

        // Dynamic viewport state
        VkPipelineViewportStateCreateInfo viewport = {};
        viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport.viewportCount = 1;
        viewport.scissorCount = 1;
        pipeline_info.pViewportState = &viewport;

        // Vertex input
        VkPipelineVertexInputStateCreateInfo input = {};
        input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        input.vertexBindingDescriptionCount = settings.vertex.bindings.size();
        input.pVertexBindingDescriptions = settings.vertex.bindings.data();
        input.vertexAttributeDescriptionCount = settings.vertex.attributes.size();
        input.pVertexAttributeDescriptions = settings.vertex.attributes.data();
        pipeline_info.pVertexInputState = &input;

        // Input assembly
        VkPipelineInputAssemblyStateCreateInfo assembly = {};
        assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        assembly.topology = settings.topology;
        assembly.primitiveRestartEnable = VK_FALSE;
        pipeline_info.pInputAssemblyState = &assembly;

        // Rasterizer
        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.polygonMode = settings.polygon_mode;
        rasterizer.cullMode = settings.cull_mode;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.lineWidth = 1;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0;
        rasterizer.depthBiasClamp = 0;
        rasterizer.depthBiasSlopeFactor = 0;
        pipeline_info.pRasterizationState = &rasterizer;

        // Multisampling (TODO)
        VkPipelineMultisampleStateCreateInfo ms = {};
        ms.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        ms.sampleShadingEnable = VK_FALSE;
        ms.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        ms.minSampleShading = 1;
        ms.pSampleMask = nullptr;
        ms.alphaToCoverageEnable = VK_FALSE;
        ms.alphaToOneEnable = VK_FALSE;
        pipeline_info.pMultisampleState = &ms;

        // Color blending (TODO allow custom blend presets e.g., add, subtract)
        VkPipelineColorBlendAttachmentState blend_attachment = {};
        blend_attachment.blendEnable = VK_TRUE;
        blend_attachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;
        blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo blend = {};
        blend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        blend.attachmentCount = 1;
        blend.pAttachments = &blend_attachment;
        blend.logicOpEnable = VK_FALSE;
        blend.logicOp = VK_LOGIC_OP_COPY;
        pipeline_info.pColorBlendState = &blend;

        // Depth and stencil testing (TODO)
        pipeline_info.pDepthStencilState = nullptr;

        // Renderpass and layout
        pipeline_info.subpass = 0;
        pipeline_info.renderPass = pass;
        pipeline_info.layout = settings.layout;

        // As pipeline derivation
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_info.basePipelineIndex = 0;

        // Build and cache
        VkPipeline pipeline;
        VkResult_log("Create Graphics Pipeline",
                     vkCreateGraphicsPipelines(device, cache, 1, &pipeline_info, nullptr, &pipeline));
        return pipeline;
    }

    VkFramebuffer VkFramebuffer_create(VkDevice device, const FramebufferSettings &settings) {
        VkFramebufferCreateInfo framebuffer_info = {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = settings.pass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = &settings.view;
        framebuffer_info.width = settings.extent.width;
        framebuffer_info.height = settings.extent.height;
        framebuffer_info.layers = 1;

        VkFramebuffer framebuffer;
        VkResult_log("Create Framebuffer", vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffer));
        return framebuffer;
    }

    VkCommandPool VkCommandPool_create(VkDevice device, QueueFamily family) {
        VkCommandPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = family.index;

        VkCommandPool pool;
        VkResult_log("Create Command Pool", vkCreateCommandPool(device, &pool_info, nullptr, &pool));
        return pool;
    }

    void VkCommandBuffer_allocate(VkDevice device,
                                  VkCommandPool pool,
                                  VkCommandBufferLevel level,
                                  VkCommandBuffer *dst,
                                  unsigned count) {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = pool;
        alloc_info.level = level;
        alloc_info.commandBufferCount = count;

        VkResult_log("Allocate Command Buffers", vkAllocateCommandBuffers(device, &alloc_info, dst));
    }

    VkFence VkFence_create(VkDevice device) {
        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence;
        VkResult_log("Create Fence", vkCreateFence(device, &fence_info, nullptr, &fence));
        return fence;
    }

    VkSemaphore VkSemaphore_create(VkDevice device) {
        VkSemaphoreCreateInfo semaphore_info = {};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        VkResult_log("Create Semaphore", vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore));
        return semaphore;
    }
} // namespace Dynamo::Graphics::Vulkan