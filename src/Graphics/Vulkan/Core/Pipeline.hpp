#pragma once

#include <array>
#include <functional>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../../Types.hpp"
#include "./Device.hpp"
#include "./PipelineLayout.hpp"
#include "./RenderPass.hpp"
#include "./ShaderModule.hpp"
#include "./VertexDescriptor.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Supported dynamic pipeline states.
     *
     */
    static constexpr std::array<vk::DynamicState, 3> DYNAMIC_PIPELINE_STATES = {
        vk::DynamicState::eViewport,      // Size of the viewport
        vk::DynamicState::eScissor,       // Region of the viewport
        vk::DynamicState::eBlendConstants // Blending function
    };

    /**
     * @brief Wrapper class for a Vulkan rendering pipeline.
     *
     * In the typical case, there would be multiple pipelines for a
     * variety of draw configuration modes.
     *
     * However, certain pipeline states can be dynamically set by
     * commands during the draw calls (TODO: see VkDynamicState).
     *
     */
    class Pipeline {
        vk::Pipeline _handle;
        std::reference_wrapper<Device> _device;

        vk::PipelineVertexInputStateCreateInfo _vertex_input_state_info;

        vk::PipelineInputAssemblyStateCreateInfo _assembly_state_info;

        vk::Viewport _viewport;
        vk::Rect2D _scissor;
        vk::PipelineViewportStateCreateInfo _viewport_state_info;

        vk::PipelineRasterizationStateCreateInfo _rasterization_state_info;

        vk::PipelineMultisampleStateCreateInfo _multisampler_state_info;

        vk::PipelineColorBlendAttachmentState _blender_attachment;
        vk::PipelineColorBlendStateCreateInfo _blender_state_info;

        vk::PipelineDepthStencilStateCreateInfo _depth_stencil_state_info;

        vk::PipelineDynamicStateCreateInfo _dynamic_state_info;

        /**
         * @brief Create a pipeline shader stages.
         *
         * @param module Handle to vk::ShaderModule
         * @param stage  Shader stage
         * @return vk::PipelineShaderStageCreateInfo
         */
        vk::PipelineShaderStageCreateInfo
        create_shader_stage(vk::ShaderModule module,
                            vk::ShaderStageFlagBits stage);

        /**
         * @brief Create the vertex input state.
         *
         */
        void create_vertex_input_state();

        /**
         * @brief Create the input assembly state.
         *
         * @param primitive_topology
         */
        void create_assembly_state(vk::PrimitiveTopology primitive_topology);

        /**
         * @brief Create a viewport state object.
         *
         * @param extent
         */
        void create_viewport_state(const vk::Extent2D &extent);

        /**
         * @brief Create the rasterization state.
         *
         * @param polygon_mode
         */
        void create_rasterization_state(vk::PolygonMode polygon_mode);

        /**
         * @brief Create the multisampler state.
         *
         */
        void create_multisampler_state();

        /**
         * @brief Create the color blender state.
         *
         */
        void create_blender_state();

        /**
         * @brief Create the depth stencil state.
         *
         */
        void create_depth_stencil_state();

        /**
         * @brief Create the dynamic state.
         *
         */
        void create_dynamic_state();

      public:
        /**
         * @brief Construct a new Pipeline object.
         *
         * @param device             Reference to the logical device
         * @param renderpass         Reference to the render pass
         * @param layout             Pipeline layout
         * @param primitive_topology Points, lines, or triangles
         * @param polygon_mode       Fill, wireframe, or point cloud
         */
        Pipeline(Device &device,
                 RenderPass &renderpass,
                 Swapchain &swapchain,
                 PipelineLayout &layout,
                 vk::PrimitiveTopology primitive_topology,
                 vk::PolygonMode polygon_mode);

        /**
         * @brief Destroy the Pipeline object.
         *
         */
        ~Pipeline();

        /**
         * @brief Get the handle to vk::Pipeline.
         *
         * @return const vk::Pipeline&
         */
        const vk::Pipeline &get_handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan