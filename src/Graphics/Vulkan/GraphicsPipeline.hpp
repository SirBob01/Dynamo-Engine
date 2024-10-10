#pragma once

#include <vulkan/vulkan_core.h>

#include <Graphics/Vulkan/Shader.hpp>
#include <Math/Vec2.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Graphics pipeline configuration settings.
     *
     */
    struct GraphicsPipelineSettings {
        VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        VkPolygonMode polygon_mode = VK_POLYGON_MODE_FILL;
        VkCullModeFlags cull_mode = VK_CULL_MODE_BACK_BIT;

        Shader vertex;
        Shader fragment;

        // TODO This should also be cached separately as this depends on the
        // available shaders
        VkPipelineLayout layout;

        // This can be configured as depthWrite, stencilWrite, etc.
        VkRenderPass renderpass;
        unsigned subpass = 0;
    };

    /**
     * @brief Render pass configuration settings.
     *
     * TODO: Support depth-stencil
     *
     */
    struct RenderPassSettings {
        VkFormat color_format;

        bool clear_color = false;
    };

    /**
     * @brief Build a Vulkan framebuffer.
     *
     * @param device
     * @param renderpass
     * @param view
     * @param extent
     * @return VkFramebuffer
     */
    VkFramebuffer VkFramebuffer_build(VkDevice device, VkRenderPass renderpass, VkImageView view, VkExtent2D extent);

    /**
     * @brief Build a Vulkan render pass
     *
     * @param device
     * @param settings
     * @return VkRenderPass
     */
    VkRenderPass VkRenderPass_build(VkDevice device, RenderPassSettings settings);

    /**
     * @brief Build a Vulkan pipeline layout.
     *
     * @param device
     * @return VkPipelineLayout
     */
    VkPipelineLayout VkPipelineLayout_build(VkDevice device);

    /**
     * @brief Build a Vulkan graphics pipeline from some preset configuration.
     *
     * TODO: Wrap this in a PipelineBuilder object that handles caching
     *
     * @param device
     * @param cache
     * @param settings
     * @return VkPipeline
     */
    VkPipeline VkPipeline_build(VkDevice device, VkPipelineCache cache, GraphicsPipelineSettings settings);
} // namespace Dynamo::Graphics::Vulkan