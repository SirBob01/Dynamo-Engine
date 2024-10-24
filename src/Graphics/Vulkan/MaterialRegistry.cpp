#include <Graphics/Vulkan/MaterialRegistry.hpp>
#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    MaterialRegistry::MaterialRegistry(VkDevice device, const std::string &filename) : _device(device) {
        std::ifstream ifstream;
        ifstream.open(filename, std::ios::app | std::ios::binary);
        ifstream.seekg(0, ifstream.end);
        size_t size = ifstream.tellg();
        ifstream.seekg(0, ifstream.beg);
        std::vector<char> buffer(size);
        ifstream.read(buffer.data(), size);
        ifstream.close();

        VkPipelineCacheCreateInfo cache_info;
        cache_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        cache_info.initialDataSize = size;
        cache_info.pInitialData = buffer.data();

        VkResult_log("Create Pipeline Cache", vkCreatePipelineCache(_device, &cache_info, nullptr, &_pipeline_cache));

        _ofstream.open(filename, std::ios::trunc | std::ios::binary);
    }

    VkRenderPass MaterialRegistry::build_renderpass(const RenderPassSettings &settings) const {
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
        VkResult result = vkCreateRenderPass(_device, &renderpass_info, nullptr, &renderpass);
        VkResult_log("Create Render Pass", result);
        return renderpass;
    }

    VkPipeline MaterialRegistry::build_pipeline(const GraphicsPipelineSettings &settings) const {
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
        pipeline_info.renderPass = settings.renderpass;
        pipeline_info.layout = settings.layout;

        // As pipeline derivation
        pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
        pipeline_info.basePipelineIndex = 0;

        // Build and cache
        VkPipeline pipeline;
        VkResult result = vkCreateGraphicsPipelines(_device, _pipeline_cache, 1, &pipeline_info, nullptr, &pipeline);
        VkResult_log("Create Graphics Pipeline", result);
        return pipeline;
    }

    Material MaterialRegistry::build(const MaterialDescriptor &descriptor,
                                     const Swapchain &swapchain,
                                     const ShaderSet &shaders) {
        MaterialInstance instance;
        const ShaderModule &vertex_module = shaders.get(descriptor.vertex);
        const ShaderModule &fragment_module = shaders.get(descriptor.fragment);

        // Aggregate descriptor layouts
        std::vector<VkDescriptorSetLayout> descriptor_layouts;
        for (const VkDescriptorSetLayout &layout : vertex_module.descriptor_layouts) {
            descriptor_layouts.push_back(layout);
        }
        for (const VkDescriptorSetLayout &layout : fragment_module.descriptor_layouts) {
            descriptor_layouts.push_back(layout);
        }

        // Aggregate push constant ranges
        std::vector<VkPushConstantRange> push_constant_ranges;
        for (const VkPushConstantRange &range : vertex_module.push_constant_ranges) {
            push_constant_ranges.push_back(range);
        }
        for (const VkPushConstantRange &range : fragment_module.push_constant_ranges) {
            push_constant_ranges.push_back(range);
        }

        // Build pipeline layout
        PipelineLayoutSettings pipeline_layout_settings;
        pipeline_layout_settings.descriptor_layouts = descriptor_layouts.data();
        pipeline_layout_settings.descriptor_layout_count = descriptor_layouts.size();
        pipeline_layout_settings.push_constant_ranges = push_constant_ranges.data();
        pipeline_layout_settings.push_constant_range_count = push_constant_ranges.size();
        auto layout_it = _layouts.find(pipeline_layout_settings);
        if (layout_it != _layouts.end()) {
            instance.layout = layout_it->second;
        } else {
            instance.layout = VkPipelineLayout_create(_device,
                                                      pipeline_layout_settings.descriptor_layouts,
                                                      pipeline_layout_settings.descriptor_layout_count,
                                                      pipeline_layout_settings.push_constant_ranges,
                                                      pipeline_layout_settings.push_constant_range_count);
            _layouts.emplace(pipeline_layout_settings, instance.layout);
        }

        // Build render pass
        RenderPassSettings renderpass_settings;
        renderpass_settings.color_format = swapchain.surface_format.format;
        renderpass_settings.clear_color = true;
        auto renderpass_it = _renderpasses.find(renderpass_settings);
        if (renderpass_it != _renderpasses.end()) {
            instance.renderpass = renderpass_it->second;
        } else {
            instance.renderpass = build_renderpass(renderpass_settings);
            _renderpasses.emplace(renderpass_settings, instance.renderpass);
        }

        // Build pipeline
        GraphicsPipelineSettings pipeline_settings;
        pipeline_settings.vertex = vertex_module;
        pipeline_settings.fragment = fragment_module;
        pipeline_settings.topology = convert_topology(descriptor.topology);
        pipeline_settings.cull_mode = convert_cull(descriptor.cull);
        pipeline_settings.polygon_mode = convert_fill(descriptor.fill);
        pipeline_settings.renderpass = instance.renderpass;
        pipeline_settings.layout = instance.layout;
        auto pipeline_it = _pipelines.find(pipeline_settings);
        if (pipeline_it != _pipelines.end()) {
            instance.pipeline = pipeline_it->second;
        } else {
            instance.pipeline = build_pipeline(pipeline_settings);
            _pipelines.emplace(pipeline_settings, instance.pipeline);
        }

        Material material = IdGenerator<Material>::generate();
        _instances.emplace(material, instance);
        return material;
    }

    MaterialInstance &MaterialRegistry::get(Material material) { return _instances.at(material); }

    void MaterialRegistry::destroy() {
        // Clean up pipelines
        vkDestroyPipelineCache(_device, _pipeline_cache, nullptr);
        for (const auto &[key, pipeline] : _pipelines) {
            vkDestroyPipeline(_device, pipeline, nullptr);
        }
        _pipelines.clear();

        // Clean up pipeline layouts
        for (const auto &[key, layout] : _layouts) {
            vkDestroyPipelineLayout(_device, layout, nullptr);
        }
        _layouts.clear();

        // Clean up renderpasses
        for (const auto &[key, renderpass] : _renderpasses) {
            vkDestroyRenderPass(_device, renderpass, nullptr);
        }
        _layouts.clear();

        // Clear all instances
        _instances.clear();
    }

    void MaterialRegistry::write_to_disk() {
        size_t size;
        vkGetPipelineCacheData(_device, _pipeline_cache, &size, nullptr);
        std::vector<char> buffer(size);
        vkGetPipelineCacheData(_device, _pipeline_cache, &size, buffer.data());

        _ofstream.write(buffer.data(), buffer.size());
    }
} // namespace Dynamo::Graphics::Vulkan