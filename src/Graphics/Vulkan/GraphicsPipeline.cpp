#include <Graphics/Vulkan/GraphicsPipeline.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    VkPipelineShaderStageCreateInfo build_shader_stage(const Shader &shader) {
        VkPipelineShaderStageCreateInfo stage_info = {};
        stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage_info.stage = shader.stage;
        stage_info.module = shader.handle;
        stage_info.pName = "main"; // Program entry-point
        return stage_info;
    }

    VkFramebuffer VkFramebuffer_build(VkDevice device, VkRenderPass renderpass, VkImageView view, VkExtent2D extent) {
        VkFramebufferCreateInfo framebuffer_info = {};
        framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebuffer_info.renderPass = renderpass;
        framebuffer_info.attachmentCount = 1;
        framebuffer_info.pAttachments = &view;
        framebuffer_info.width = extent.width;
        framebuffer_info.height = extent.height;
        framebuffer_info.layers = 1;

        VkFramebuffer framebuffer;
        VkResult_log("Create Framebuffer", vkCreateFramebuffer(device, &framebuffer_info, nullptr, &framebuffer));
        return framebuffer;
    }

    VkRenderPass VkRenderPass_build(VkDevice device, RenderPassSettings settings) {
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

    VkPipelineLayout VkPipelineLayout_build(VkDevice device) {
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

    VkPipeline VkPipeline_build(VkDevice device, VkPipelineCache cache, GraphicsPipelineSettings settings) {
        VkGraphicsPipelineCreateInfo pipeline_info = {};
        pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        // Dynamic pipeline states that need to be set during command recording
        std::array<VkDynamicState, 5> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR,
            VK_DYNAMIC_STATE_BLEND_CONSTANTS,
            VK_DYNAMIC_STATE_DEPTH_BOUNDS,
            // This is the only one that may not be fully supported...
            VK_DYNAMIC_STATE_LINE_WIDTH,
        };
        VkPipelineDynamicStateCreateInfo dynamic = {};
        dynamic.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic.dynamicStateCount = dynamic_states.size();
        dynamic.pDynamicStates = dynamic_states.data();
        pipeline_info.pDynamicState = &dynamic;

        // Programmable shader stages
        std::array<VkPipelineShaderStageCreateInfo, 2> stages = {
            build_shader_stage(settings.vertex),
            build_shader_stage(settings.fragment),
        };
        pipeline_info.stageCount = stages.size();
        pipeline_info.pStages = stages.data();

        // Dynamic viewport state
        VkPipelineViewportStateCreateInfo viewport = {};
        viewport.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport.viewportCount = 1;
        viewport.scissorCount = 1;
        pipeline_info.pViewportState = &viewport;

        // Vertex input (TODO)
        VkPipelineVertexInputStateCreateInfo input = {};
        input.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        input.vertexBindingDescriptionCount = 0;
        input.pVertexBindingDescriptions = nullptr;
        input.vertexAttributeDescriptionCount = 0;
        input.pVertexAttributeDescriptions = nullptr;
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
        pipeline_info.renderPass = settings.renderpass;
        pipeline_info.subpass = settings.subpass;
        pipeline_info.layout = settings.layout;

        // As pipeline derivation
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_info.basePipelineIndex = 0;

        VkPipeline pipeline;
        VkResult_log("Create Graphics Pipeline",
                     vkCreateGraphicsPipelines(device, cache, 1, &pipeline_info, nullptr, &pipeline));
        return pipeline;
    }
} // namespace Dynamo::Graphics::Vulkan