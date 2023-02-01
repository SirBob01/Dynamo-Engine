#include "./DrawSet.hpp"

namespace Dynamo::Graphics::Vulkan {
    DrawSet::DrawSet(Swapchain &swapchain,
                     CommandPool &command_pool,
                     Buffer &object_buffer,
                     vk::Queue graphics_queue) :
        _swapchain(swapchain),
        _command_pool(command_pool), _object_buffer(object_buffer),
        _queue(graphics_queue) {
        _command_buffers =
            _command_pool.get().allocate(vk::CommandBufferLevel::ePrimary,
                                         _swapchain.get().get_images().size());
        _color_clear.color.setFloat32({0, 0, 0, 1});
        _depth_clear.depthStencil.depth = 1;
        _depth_clear.depthStencil.stencil = 0;
    }

    void DrawSet::add(MeshInstance &mesh_instance,
                      Material &material,
                      std::vector<Mat4> &transforms) {
        InstanceGroup group = {
            mesh_instance,
            static_cast<u32>(transforms.size()),
        };
        _set[&material].push_back(group);
    }

    void DrawSet::clear(Color &color) {
        _color_clear.color.setFloat32(color.to_array());
    }

    vk::CommandBuffer &DrawSet::get_command_buffer(u32 frame) {
        return *_command_buffers[frame];
    }

    void DrawSet::record() {
        _queue.waitIdle();
        _command_pool.get().reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        vk::Rect2D render_area;
        render_area.offset.x = 0;
        render_area.offset.y = 0;
        render_area.extent = _swapchain.get().get_extent();

        std::array<vk::ClearValue, 2> clear_values = {
            _color_clear,
            _depth_clear,
        };

        vk::CommandBufferBeginInfo begin_info;
        for (u32 i = 0; i < _command_buffers.size(); i++) {
            // Start recording
            vk::CommandBuffer &command_buffer = *_command_buffers[i];
            command_buffer.begin(begin_info);

            // Set the dynamic states
            vk::Viewport viewport;
            viewport.width = render_area.extent.width;
            viewport.height = render_area.extent.height;
            viewport.x = 0;
            viewport.y = 0;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            command_buffer.setViewport(0, viewport);

            vk::Rect2D scissor;
            scissor.offset.x = 0;
            scissor.offset.y = 0;
            scissor.extent = render_area.extent;
            command_buffer.setScissor(0, scissor);

            // Process each draw group
            for (auto [material, groups] : _set) {
                // Get the renderpass and its framebfuffers
                const RenderPass &renderpass = material->get_renderpass();
                const std::vector<vk::Framebuffer> &framebuffers =
                    renderpass.get_framebuffers();

                // Begin the renderpass
                vk::RenderPassBeginInfo render_begin_info;
                render_begin_info.renderPass = renderpass.get_handle();
                render_begin_info.framebuffer = framebuffers[i];
                render_begin_info.renderArea = render_area;
                render_begin_info.clearValueCount = clear_values.size();
                render_begin_info.pClearValues = clear_values.data();

                command_buffer.beginRenderPass(render_begin_info,
                                               vk::SubpassContents::eInline);

                // Bind the command buffer to the graphics pipeline
                command_buffer.bindPipeline(
                    vk::PipelineBindPoint::eGraphics,
                    material->get_pipeline().get_handle());

                // Grab the handles of the descriptor sets for this frame
                const std::vector<vk::DescriptorSet> &descriptor_sets =
                    material->get_descriptor_set_groups()[i];

                // Bind the descriptor sets
                if (descriptor_sets.size() > 0) {
                    command_buffer.bindDescriptorSets(
                        vk::PipelineBindPoint::eGraphics,
                        material->get_layout().get_handle(),
                        0,
                        descriptor_sets,
                        nullptr);
                }

                for (InstanceGroup &group : groups) {
                    // Bind the vertex block from the object buffer
                    command_buffer.bindVertexBuffers(
                        0,
                        _object_buffer.get().get_handle(),
                        group.mesh_instance.get().vertex_offset());

                    // Bind the index block from the object buffer
                    command_buffer.bindIndexBuffer(
                        _object_buffer.get().get_handle(),
                        group.mesh_instance.get().index_offset(),
                        vk::IndexType::eUint32);

                    // Draw the instanced mesh
                    command_buffer.drawIndexed(
                        group.mesh_instance.get().index_count(),
                        group.count,
                        0,
                        0,
                        0);
                }

                // End the renderpass
                command_buffer.endRenderPass();
            }

            // Stop recording
            command_buffer.end();
        }

        // Clear the set
        _set.clear();
    }
} // namespace Dynamo::Graphics::Vulkan