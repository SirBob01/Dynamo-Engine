#pragma once

#include <functional>
#include <string>

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Image view configuration settings.
     *
     */
    struct ImageViewSettings {
        VkFormat format = VK_FORMAT_B8G8R8A8_SRGB;
        VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D;
        VkImageAspectFlags aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;

        unsigned mip_base = 0;
        unsigned mip_count = 1;

        unsigned layer_base = 0;
        unsigned layer_count = 1;
    };

    /**
     * @brief Render pass configuration settings.
     *
     */
    struct RenderPassSettings {
        VkFormat color_format;
        VkFormat depth_format;

        bool clear_color = false;
        bool clear_depth = false;

        unsigned sample_count = 1;

        bool operator==(const RenderPassSettings &other) const;
    };

    /**
     * @brief Shader key.
     *
     */
    struct ShaderKey {
        std::string code;
        VkShaderStageFlagBits stage;

        bool operator==(const ShaderKey &other) const;
    };

    /**
     * @brief Graphics pipeline configuration settings.
     *
     */
    struct GraphicsPipelineSettings {
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL;
        VkCullModeFlags cull_mode = VK_CULL_MODE_BACK_BIT;

        VkShaderModule vertex;
        VkShaderModule fragment;

        RenderPassSettings renderpass;

        // TODO: Should be determined from shader reflection
        VkPipelineLayout layout;

        bool operator==(const GraphicsPipelineSettings &other) const;
    };

    /**
     * @brief Convert VkPhysicalDeviceType to string.
     *
     * @param type
     * @return const char*
     */
    const char *VkPhysicalDeviceType_string(VkPhysicalDeviceType type);

    /**
     * @brief Convert VkShaderStageFlagBits to string.
     *
     * @param stage
     * @return const char*
     */
    const char *VkShaderStageFlagBits_string(VkShaderStageFlagBits stage);

    /**
     * @brief Convert VkResult to string.
     *
     * @param result
     * @return const char*
     */
    const char *VkResult_string(VkResult result);

    /**
     * @brief Log the result of a Vulkan operation.
     *
     * @param op_message
     * @param result
     */
    void VkResult_log(const std::string &op_message, VkResult result);

    /**
     * @brief Create a Vulkan instance.
     *
     * @param display
     * @return VkInstance
     */
    VkInstance VkInstance_create(const Display &display);

    /**
     * @brief Create a Vulkan debug messenger.
     *
     * @param instance
     * @return VkDebugUtilsMessengerEXT
     */
    VkDebugUtilsMessengerEXT VkDebugUtilsMessengerEXT_create(VkInstance instance);

    /**
     * @brief Create the Vulkan logical device.
     *
     * @param physical
     * @return VkDevice
     */
    VkDevice VkDevice_create(const PhysicalDevice &physical);

    /**
     * @brief Create a Vulkan image view.
     *
     * @param device
     * @param image
     * @param settings
     * @return VkImageView
     */
    VkImageView VkImageView_create(VkDevice device, VkImage image, ImageViewSettings settings);

    /**
     * @brief Create a Vulkan render pass.
     *
     * @param device
     * @param settings
     * @return VkRenderPass
     */
    VkRenderPass VkRenderPass_create(VkDevice device, const RenderPassSettings &settings);

    /**
     * @brief Create a Vulkan pipeline layout.
     *
     * @param device
     * @return VkPipelineLayout
     */
    VkPipelineLayout VkPipelineLayout_create(VkDevice device);

    /**
     * @brief Create a Vulkan shader module.
     *
     * @param device
     * @param bytecode
     * @return VkShaderModule
     */
    VkShaderModule VkShaderModule_create(VkDevice device, const std::vector<uint32_t> &bytecode);

    /**
     * @brief Create a Vulkan graphics pipeline.
     *
     * @param device
     * @param cache
     * @param pass
     * @param settings
     * @return VkPipeline
     */
    VkPipeline VkPipeline_create(VkDevice device,
                                 VkPipelineCache cache,
                                 VkRenderPass pass,
                                 const GraphicsPipelineSettings &settings);

    /**
     * @brief Create a Vulkan framebuffer.
     *
     * @param device
     * @param renderpass
     * @param view
     * @param extent
     * @return VkFramebuffer
     */
    VkFramebuffer VkFramebuffer_create(VkDevice device, VkRenderPass renderpass, VkImageView view, VkExtent2D extent);

    /**
     * @brief Create a Vulkan command pool.
     *
     * @param device
     * @param family
     * @return VkCommandPool
     */
    VkCommandPool VkCommandPool_create(VkDevice device, QueueFamily family);

    /**
     * @brief Allocate Vulkan command buffers from a pool.
     *
     * @param device
     * @param pool
     * @param level
     * @param count
     * @return std::vector<VkCommandBuffer>
     */
    std::vector<VkCommandBuffer>
    VkCommandBuffer_allocate(VkDevice device, VkCommandPool pool, VkCommandBufferLevel level, unsigned count);

    /**
     * @brief Create a Vulkan fence.
     *
     * @param device
     * @return VkFence
     */
    VkFence VkFence_create(VkDevice device);

    /**
     * @brief Create a Vulkan semaphore.
     *
     * @param device
     * @return VkSemaphore
     */
    VkSemaphore VkSemaphore_create(VkDevice device);
} // namespace Dynamo::Graphics::Vulkan

//// Hash function definitions for key structs.
//// TODO: Come up with better hash functions?

template <>
struct std::hash<Dynamo::Graphics::Vulkan::RenderPassSettings> {
    inline size_t operator()(const Dynamo::Graphics::Vulkan::RenderPassSettings &settings) const {
        size_t hash0 = std::hash<bool>{}(settings.clear_color);
        size_t hash1 = std::hash<bool>{}(settings.clear_depth);

        size_t hash2 = std::hash<unsigned>{}(settings.color_format);
        size_t hash3 = std::hash<unsigned>{}(settings.depth_format);

        size_t hash4 = std::hash<unsigned>{}(settings.sample_count);

        return hash0 ^ (hash1 << 1) ^ (hash2 << 2) ^ (hash3 << 3) ^ (hash4 << 4);
    }
};

template <>
struct std::hash<Dynamo::Graphics::Vulkan::ShaderKey> {
    inline size_t operator()(const Dynamo::Graphics::Vulkan::ShaderKey &key) const {
        size_t hash0 = std::hash<std::string>{}(key.code);
        size_t hash1 = std::hash<unsigned>{}(key.stage);
        return hash0 ^ (hash1 + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2));
    }
};

template <>
struct std::hash<Dynamo::Graphics::Vulkan::GraphicsPipelineSettings> {
    inline size_t operator()(const Dynamo::Graphics::Vulkan::GraphicsPipelineSettings &settings) const {
        size_t hash0 = std::hash<unsigned>{}(settings.topology);
        size_t hash1 = std::hash<unsigned>{}(settings.polygon_mode);
        size_t hash2 = std::hash<unsigned>{}(settings.cull_mode);
        size_t hash3 = std::hash<void *>{}(settings.vertex);
        size_t hash4 = std::hash<void *>{}(settings.fragment);
        size_t hash5 = std::hash<Dynamo::Graphics::Vulkan::RenderPassSettings>{}(settings.renderpass);
        size_t hash6 = std::hash<void *>{}(settings.layout);

        return hash0 ^ (hash1 << 1) ^ (hash2 << 2) ^ (hash3 << 3) ^ (hash4 << 4) ^ (hash5 << 5) ^ (hash6 << 6);
    }
};