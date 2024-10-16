#include <Graphics/Renderer.hpp>
#include <Graphics/Vulkan/Utils.hpp>

static const char *TRIANGLE_VERTEX_SHADER = R"(
#version 450

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}
)";

static const char *TRIANGLE_FRAGMENT_SHADER = R"(
#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
)";

// TODO:
// * Vertex attribute handling <- Vertex attributes should be determined from shader reflection (bindings)
//                             <- Deinterleaved, 1 binding per vertex attribute

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(const Display &display, const std::string &root_asset_directory) : _display(display) {
        _instance = VkInstance_create(_display);
#ifdef DYN_DEBUG
        _debugger = VkDebugUtilsMessengerEXT_create(_instance);
#endif
        _surface = _display.create_vulkan_surface(_instance);

        // Create the logical device
        _physical = PhysicalDevice::select(_instance, _surface);
        _device = VkDevice_create(_physical);

        // Build the swapchain and its views
        _swapchain = Swapchain(_device, _physical, _display);

        // Vulkan object caches
        _shader_cache = ShaderCache(_device);
        _pipeline_cache = PipelineCache(_device, root_asset_directory + "/vulkan_cache.bin");
        _framebuffer_cache = FramebufferCache(_device);

        // Command buffer pools for each queue family
        _graphics_pool = VkCommandPool_create(_device, _physical.graphics_queues);
        _transfer_pool = VkCommandPool_create(_device, _physical.transfer_queues);

        // Frame contexts
        _frame_contexts = FrameContextList<3>(_device, _graphics_pool);

        // Main buffers
        std::array<VkCommandBuffer, 3> transfer_buffers;
        VkCommandBuffer_allocate(_device, _transfer_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, transfer_buffers.data(), 3);
        _vertex_buffer = Buffer(_device,
                                _physical,
                                transfer_buffers[0],
                                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        _index_buffer = Buffer(_device,
                               _physical,
                               transfer_buffers[1],
                               VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                               VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        _staging_buffer = Buffer(_device,
                                 _physical,
                                 transfer_buffers[2],
                                 VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT |
                                     VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // Color fill clear value
        _clear.color.float32[0] = 0;
        _clear.color.float32[1] = 0;
        _clear.color.float32[2] = 0;
        _clear.color.float32[3] = 1;

        /** Demo Triangle rendering specific objects */

        _layout = VkPipelineLayout_create(_device);

        GraphicsPipelineSettings pipeline_settings;
        pipeline_settings.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipeline_settings.cull_mode = VK_CULL_MODE_NONE;
        pipeline_settings.polygon_mode = VK_POLYGON_MODE_FILL;
        pipeline_settings.layout = _layout;

        pipeline_settings.renderpass.clear_color = true;
        pipeline_settings.renderpass.color_format = _swapchain.surface_format.format;

        pipeline_settings.vertex = _shader_cache.build({TRIANGLE_VERTEX_SHADER, VK_SHADER_STAGE_VERTEX_BIT});
        pipeline_settings.fragment = _shader_cache.build({TRIANGLE_FRAGMENT_SHADER, VK_SHADER_STAGE_FRAGMENT_BIT});
        _pipeline_pass = _pipeline_cache.build(pipeline_settings);
    }

    Renderer::~Renderer() {
        // Wait for device queues to finish processing
        vkDeviceWaitIdle(_device);

        // Cleanup Triangle Demo objects
        vkDestroyPipelineLayout(_device, _layout, nullptr);

        // Cleanup high level objects
        _framebuffer_cache.destroy();
        _pipeline_cache.destroy();
        _shader_cache.destroy();
        _frame_contexts.destroy();
        _vertex_buffer.destroy();
        _index_buffer.destroy();
        _staging_buffer.destroy();
        _swapchain.destroy();

        vkDestroyCommandPool(_device, _graphics_pool, nullptr);
        vkDestroyCommandPool(_device, _transfer_pool, nullptr);
        vkDestroyDevice(_device, nullptr);
        vkDestroySurfaceKHR(_instance, _surface, nullptr);
#ifdef DYN_DEBUG
        vkDestroyDebugUtilsMessengerEXT(_instance, _debugger, nullptr);
#endif
        vkDestroyInstance(_instance, nullptr);
    }

    void Renderer::rebuild_swapchain() {
        // Destroy old swapchain resources
        vkDeviceWaitIdle(_device);
        _framebuffer_cache.destroy();

        // Rebuild the swapchain
        _swapchain = Swapchain(_device, _physical, _display, _swapchain);
    }

    void Renderer::set_clear(Color color) {
        _clear.color.float32[0] = color.r;
        _clear.color.float32[1] = color.g;
        _clear.color.float32[2] = color.b;
        _clear.color.float32[3] = color.a;
    }

    void Renderer::refresh() {
        const FrameContext &frame = _frame_contexts.next();
        vkWaitForFences(_device, 1, &frame.sync_fence, VK_TRUE, UINT64_MAX);

        unsigned image_index;
        VkResult acquire_result = vkAcquireNextImageKHR(_device,
                                                        _swapchain.handle,
                                                        UINT64_MAX,
                                                        frame.sync_render_start,
                                                        VK_NULL_HANDLE,
                                                        &image_index);
        if (acquire_result == VK_ERROR_OUT_OF_DATE_KHR) {
            rebuild_swapchain();
            return;
        } else if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR) {
            VkResult_log("Acquire Image", acquire_result);
        }

        VkResult_log("Reset Fence", vkResetFences(_device, 1, &frame.sync_fence));
        VkResult_log("Reset Command Buffer", vkResetCommandBuffer(frame.command_buffer, 0));

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0;
        begin_info.pInheritanceInfo = nullptr;

        VkResult_log("Begin Command Recording", vkBeginCommandBuffer(frame.command_buffer, &begin_info));

        VkViewport viewport;
        viewport.minDepth = 0;
        viewport.maxDepth = 1;
        viewport.width = _swapchain.extent.width;
        viewport.height = _swapchain.extent.height;
        viewport.x = 0;
        viewport.y = 0;
        vkCmdSetViewport(frame.command_buffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.extent = _swapchain.extent;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        vkCmdSetScissor(frame.command_buffer, 0, 1, &scissor);

        // -- TODO: Factor these out into model draw commands
        FramebufferSettings framebuffer_settings;
        framebuffer_settings.view = _swapchain.views[image_index];
        framebuffer_settings.extent = _swapchain.extent;
        framebuffer_settings.pass = _pipeline_pass.pass;

        VkRenderPassBeginInfo renderpass_begin_info = {};
        renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderPass = _pipeline_pass.pass;
        renderpass_begin_info.renderArea.extent = _swapchain.extent;
        renderpass_begin_info.renderArea.offset.x = 0;
        renderpass_begin_info.renderArea.offset.y = 0;
        renderpass_begin_info.clearValueCount = 1;
        renderpass_begin_info.pClearValues = &_clear;
        renderpass_begin_info.framebuffer = _framebuffer_cache.build(framebuffer_settings);
        vkCmdBeginRenderPass(frame.command_buffer, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(frame.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_pass.pipeline);
        vkCmdDraw(frame.command_buffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(frame.command_buffer);
        // --

        VkResult_log("End Command Buffer", vkEndCommandBuffer(frame.command_buffer));

        VkQueue queue;
        vkGetDeviceQueue(_device, _physical.graphics_queues.index, 0, &queue);

        VkSubmitInfo submit_info = {};
        VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &frame.command_buffer;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &frame.sync_render_start;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &frame.sync_render_done;
        submit_info.pWaitDstStageMask = &wait_stage_mask;

        VkResult_log("Graphics Submit", vkQueueSubmit(queue, 1, &submit_info, frame.sync_fence));

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &frame.sync_render_done;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &_swapchain.handle;
        present_info.pImageIndices = &image_index;
        present_info.pResults = nullptr;

        VkResult present_result = vkQueuePresentKHR(queue, &present_info);

        if (present_result == VK_ERROR_OUT_OF_DATE_KHR || present_result == VK_SUBOPTIMAL_KHR) {
            rebuild_swapchain();
        } else if (present_result != VK_SUCCESS) {
            VkResult_log("Present Render", present_result);
        }
    }
} // namespace Dynamo::Graphics::Vulkan