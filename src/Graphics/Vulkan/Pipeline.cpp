#include "./Pipeline.hpp"

namespace Dynamo::Graphics::Vulkan {
    Pipeline::Pipeline(Device &device,
                       RenderPass &renderpass,
                       Swapchain &swapchain,
                       PipelineLayout &layout,
                       vk::PrimitiveTopology primitive_topology,
                       vk::PolygonMode polygon_mode) :
        _device(device) {
        create_vertex_input_state();
        create_assembly_state(primitive_topology);
        create_viewport_state(swapchain.get_extent());
        create_rasterization_state(polygon_mode);
        create_multisampler_state();
        create_depth_stencil_state();
        create_blender_state();
        create_dynamic_state();

        vk::GraphicsPipelineCreateInfo pipeline_info;

        // Assembly stages
        pipeline_info.pVertexInputState = &_vertex_input_state_info;
        pipeline_info.pInputAssemblyState = &_assembly_state_info;

        // Fixed function stages
        pipeline_info.pViewportState = &_viewport_state_info;
        pipeline_info.pRasterizationState = &_rasterization_state_info;
        pipeline_info.pMultisampleState = &_multisampler_state_info;
        pipeline_info.pDepthStencilState = &_depth_stencil_state_info;
        pipeline_info.pColorBlendState = &_blender_state_info;
        pipeline_info.pDynamicState = &_dynamic_state_info;

        // Programmable shader stages
        std::vector<vk::UniqueShaderModule> shader_modules;
        std::vector<vk::PipelineShaderStageCreateInfo> shader_stage_infos;
        const ShaderList &shaders = layout.get_shaders();
        for (u32 i = 0; i < shaders.size(); i++) {
            shader_modules.push_back(shaders[i].get().get_handle());
            shader_stage_infos.push_back(
                create_shader_stage(shader_modules[i].get(),
                                    shaders[i].get().get_stage()));
        }
        pipeline_info.stageCount = shader_stage_infos.size();
        pipeline_info.pStages = shader_stage_infos.data();

        // Layout and render pass
        pipeline_info.layout = layout.get_handle();
        pipeline_info.renderPass = renderpass.get_handle();
        pipeline_info.subpass = 0; // Subpass index where it is used

        // Derived pipeline?
        pipeline_info.basePipelineHandle = nullptr;
        pipeline_info.basePipelineIndex = 0;

        vk::ResultValue<vk::Pipeline> result =
            _device.get().get_handle().createGraphicsPipeline(
                nullptr, // TODO: Pipeline cache
                pipeline_info);
        _handle = result.value;
    }

    Pipeline::~Pipeline() {
        _device.get().get_handle().destroyPipeline(_handle);
    }

    vk::PipelineShaderStageCreateInfo
    Pipeline::create_shader_stage(vk::ShaderModule module,
                                  vk::ShaderStageFlagBits stage) {
        vk::PipelineShaderStageCreateInfo shader_stage;
        shader_stage.module = module;
        shader_stage.stage = stage;
        shader_stage.pName = "main"; // Entry function

        return shader_stage;
    }

    void Pipeline::create_vertex_input_state() {
        _vertex_input_state_info.vertexBindingDescriptionCount = 1;
        _vertex_input_state_info.pVertexBindingDescriptions =
            &VERTEX_BINDING_DESCRIPTION;
        _vertex_input_state_info.vertexAttributeDescriptionCount =
            VERTEX_ATTRIBUTE_DESCRIPTIONS.size();
        _vertex_input_state_info.pVertexAttributeDescriptions =
            VERTEX_ATTRIBUTE_DESCRIPTIONS.data();
    }

    void
    Pipeline::create_assembly_state(vk::PrimitiveTopology primitive_topology) {
        _assembly_state_info.topology = primitive_topology;
        _assembly_state_info.primitiveRestartEnable = false;
    }

    void Pipeline::create_viewport_state(const vk::Extent2D &extent) {
        // Origin
        _viewport.x = 0.0f;
        _viewport.y = 0.0f;

        // Dimensions
        _viewport.width = extent.width;
        _viewport.height = extent.height;

        // Render depth range
        _viewport.minDepth = 0.0f;
        _viewport.maxDepth = 1.0f;

        // Subsection of the viewport to be used
        _scissor.offset.x = 0;
        _scissor.offset.y = 0;
        _scissor.extent = extent;

        _viewport_state_info.viewportCount = 1;
        _viewport_state_info.pViewports = &_viewport;
        _viewport_state_info.scissorCount = 1;
        _viewport_state_info.pScissors = &_scissor;
    }

    void Pipeline::create_rasterization_state(vk::PolygonMode polygon_mode) {
        _rasterization_state_info.depthClampEnable = false;
        _rasterization_state_info.rasterizerDiscardEnable = false;

        // How to draw polygon
        // * eFill - Standard
        // * eLine - Wireframe
        // * ePoint - Point cloud
        _rasterization_state_info.polygonMode = polygon_mode;
        _rasterization_state_info.lineWidth = 1.0;

        // Backface culling?
        _rasterization_state_info.cullMode = vk::CullModeFlagBits::eBack;
        _rasterization_state_info.frontFace = vk::FrontFace::eCounterClockwise;

        // Manipulate the depth values?
        // TODO: Could be useful for shadow mapping
        _rasterization_state_info.depthBiasEnable = false;
        _rasterization_state_info.depthBiasConstantFactor = 0.0f;
        _rasterization_state_info.depthBiasClamp = 0.0f;
        _rasterization_state_info.depthBiasSlopeFactor = 0.0f;
    }

    void Pipeline::create_multisampler_state() {
        _multisampler_state_info.rasterizationSamples =
            _device.get().get_physical().get_msaa_samples();
        _multisampler_state_info.sampleShadingEnable = true;

        // Multisampling rate for anti-aliasing the texture
        _multisampler_state_info.minSampleShading = 0.5f;
    }

    void Pipeline::create_blender_state() {
        // TODO: Allow custom blend function? Add, subtract, multiply, etc.
        _blender_attachment.blendEnable = true;
        _blender_attachment.colorWriteMask =
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
            vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

        // RGB blending operation
        _blender_attachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        _blender_attachment.dstColorBlendFactor =
            vk::BlendFactor::eOneMinusSrcAlpha;

        // Alpha blending operation
        _blender_attachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        _blender_attachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        _blender_attachment.alphaBlendOp = vk::BlendOp::eAdd;

        // Create the blender state
        _blender_state_info.logicOpEnable = false;
        _blender_state_info.logicOp = vk::LogicOp::eNoOp;
        _blender_state_info.attachmentCount = 1;
        _blender_state_info.pAttachments = &_blender_attachment;
    }

    void Pipeline::create_depth_stencil_state() {
        _depth_stencil_state_info.depthTestEnable = true;
        _depth_stencil_state_info.depthWriteEnable = true;
        _depth_stencil_state_info.depthCompareOp = vk::CompareOp::eLess;
        _depth_stencil_state_info.depthBoundsTestEnable = false;
        _depth_stencil_state_info.stencilTestEnable = false;
    }

    void Pipeline::create_dynamic_state() {
        _dynamic_state_info.dynamicStateCount = DYNAMIC_PIPELINE_STATES.size();
        _dynamic_state_info.pDynamicStates = DYNAMIC_PIPELINE_STATES.data();
    }

    const vk::Pipeline &Pipeline::get_handle() const { return _handle; }
} // namespace Dynamo::Graphics::Vulkan