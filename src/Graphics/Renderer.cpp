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

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(const Display &display, const std::string &pipeline_cache_filename) : _display(display) {
        _instance = VkInstance_create(_display);
#ifdef DYN_DEBUG
        _debugger = VkDebugUtilsMessengerEXT_create(_instance);
#endif
        _surface = _display.create_vulkan_surface(_instance);

        // Create the logical device
        _physical = PhysicalDevice::select(_instance, _surface);
        _device = VkDevice_create(_physical);

        // Build the swapchain and its views and framebuffers
        _swapchain = Swapchain(_device, _physical, _display);

        // Color fill clear value
        _clear.color.float32[0] = 0;
        _clear.color.float32[1] = 0;
        _clear.color.float32[2] = 0;
        _clear.color.float32[3] = 1;

        // Vulkan object caches
        _shader_cache = ShaderCache(_device);
        _pipeline_cache = PipelineCache(_device, pipeline_cache_filename);

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

        for (VkImage image : _swapchain.images) {
            ImageViewSettings view_settings;
            view_settings.format = _swapchain.surface_format.format;

            VkImageView view = VkImageView_create(_device, image, view_settings);
            _views.push_back(view);

            VkFramebuffer framebuffer = VkFramebuffer_create(_device, _pipeline_pass.pass, view, _swapchain.extent);
            _framebuffers.push_back(framebuffer);
        }

        // Command buffer pools for each queue family
        _graphics_pool = VkCommandPool_create(_device, _physical.graphics_queues);

        // Frame sync objects
        std::vector<VkCommandBuffer> command_buffers =
            VkCommandBuffer_allocate(_device, _graphics_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, _frame_context.size());
        for (unsigned i = 0; i < _frame_context.size(); i++) {
            _frame_context[i].sync_fence = VkFence_create(_device);
            _frame_context[i].sync_render_start = VkSemaphore_create(_device);
            _frame_context[i].sync_render_done = VkSemaphore_create(_device);
            _frame_context[i].command_buffer = command_buffers[i];
        }
        _current_frame = 0;
    }

    Renderer::~Renderer() {
        // Wait for device queues to finish processing
        vkDeviceWaitIdle(_device);

        // Cleanup Triangle Demo objects
        _pipeline_cache.destroy();
        _shader_cache.destroy();
        vkDestroyPipelineLayout(_device, _layout, nullptr);

        // Cleanup
        vkDestroyCommandPool(_device, _graphics_pool, nullptr);
        for (unsigned i = 0; i < _frame_context.size(); i++) {
            vkDestroyFence(_device, _frame_context[i].sync_fence, nullptr);
            vkDestroySemaphore(_device, _frame_context[i].sync_render_start, nullptr);
            vkDestroySemaphore(_device, _frame_context[i].sync_render_done, nullptr);
        }
        for (VkFramebuffer framebuffer : _framebuffers) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        for (VkImageView view : _views) {
            vkDestroyImageView(_device, view, nullptr);
        }
        vkDestroySwapchainKHR(_device, _swapchain.handle, nullptr);
        vkDestroyDevice(_device, nullptr);
        vkDestroySurfaceKHR(_instance, _surface, nullptr);
#ifdef DYN_DEBUG
        vkDestroyDebugUtilsMessengerEXT(_instance, _debugger, nullptr);
#endif
        vkDestroyInstance(_instance, nullptr);
    }

    void Renderer::rebuild_swapchain() {
        vkDeviceWaitIdle(_device);

        VkSwapchainKHR previous = _swapchain.handle;
        _swapchain = Swapchain(_device, _physical, _display, previous);

        // Destroy old swapchain resources
        for (VkFramebuffer framebuffer : _framebuffers) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        for (VkImageView view : _views) {
            vkDestroyImageView(_device, view, nullptr);
        }
        vkDestroySwapchainKHR(_device, previous, nullptr);

        // Allocate new swapchain resources
        _views.clear();
        _framebuffers.clear();
        for (VkImage image : _swapchain.images) {
            ImageViewSettings view_settings;
            view_settings.format = _swapchain.surface_format.format;

            VkImageView view = VkImageView_create(_device, image, view_settings);
            _views.push_back(view);

            VkFramebuffer framebuffer = VkFramebuffer_create(_device, _pipeline_pass.pass, view, _swapchain.extent);
            _framebuffers.push_back(framebuffer);
        }
    }

    void Renderer::set_clear(Color color) {
        _clear.color.float32[0] = color.r;
        _clear.color.float32[1] = color.g;
        _clear.color.float32[2] = color.b;
        _clear.color.float32[3] = color.a;
    }

    void Renderer::refresh() {
        FrameContext frame = _frame_context[_current_frame];
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

        VkRenderPassBeginInfo renderpass_begin_info = {};
        renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderPass = _pipeline_pass.pass;
        renderpass_begin_info.renderArea.extent = _swapchain.extent;
        renderpass_begin_info.renderArea.offset.x = 0;
        renderpass_begin_info.renderArea.offset.y = 0;
        renderpass_begin_info.clearValueCount = 1;
        renderpass_begin_info.pClearValues = &_clear;
        renderpass_begin_info.framebuffer = _framebuffers[image_index];

        vkCmdBeginRenderPass(frame.command_buffer, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(frame.command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline_pass.pipeline);

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

        vkCmdDraw(frame.command_buffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(frame.command_buffer);
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

        _current_frame = (_current_frame + 1) % _frame_context.size();
    }
} // namespace Dynamo::Graphics::Vulkan