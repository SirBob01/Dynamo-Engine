#include "Graphics/Vulkan/Synchronize.hpp"
#include <Graphics/Renderer.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <vulkan/vulkan_core.h>

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
    Renderer::Renderer(const Display &display) : _display(display) {
        _instance = VkInstance_build(_display);
#ifdef DYN_DEBUG
        _debugger = VkDebugUtilsMessengerEXT_build(_instance);
#endif
        _surface = _display.create_vulkan_surface(_instance);

        // Build the physical device
        _physical = PhysicalDevice::select(_instance, _surface);
        _device = VkDevice_build(_physical);

        // Build the swapchain and its views and framebuffers
        _swapchain = Swapchain::build(_device, _physical, _display);

        /** Demo Triangle rendering specific objects */

        _vertex = Shader::build(_device, "Vertex", TRIANGLE_VERTEX_SHADER, VK_SHADER_STAGE_VERTEX_BIT);
        _fragment = Shader::build(_device, "Fragment", TRIANGLE_FRAGMENT_SHADER, VK_SHADER_STAGE_FRAGMENT_BIT);

        _layout = VkPipelineLayout_build(_device);

        RenderPassSettings renderpass_settings;
        renderpass_settings.clear_color = true;
        renderpass_settings.color_format = _swapchain.surface_format.format;
        _renderpass = VkRenderPass_build(_device, renderpass_settings);

        GraphicsPipelineSettings pipeline_settings;
        pipeline_settings.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipeline_settings.cull_mode = VK_CULL_MODE_NONE;
        pipeline_settings.polygon_mode = VK_POLYGON_MODE_FILL;
        pipeline_settings.layout = _layout;
        pipeline_settings.renderpass = _renderpass;
        pipeline_settings.subpass = 0;
        pipeline_settings.vertex = _vertex;
        pipeline_settings.fragment = _fragment;
        _pipeline = VkPipeline_build(_device, VK_NULL_HANDLE, pipeline_settings);

        for (VkImageView view : _swapchain.views) {
            _framebuffers.push_back(VkFramebuffer_build(_device, _renderpass, view, _swapchain.extent));
        }

        _command_pool = VkCommandPool_build(_device, _physical.graphics_queues);
        _command_buffer = VkCommandBuffer_allocate(_device, _command_pool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1)[0];

        _f_frame_ready = VkFence_build(_device);
        _s_render_start = VkSemaphore_build(_device);
        _s_render_done = VkSemaphore_build(_device);
    }

    Renderer::~Renderer() {
        // Wait for device queues to finish processing
        vkDeviceWaitIdle(_device);

        // Cleanup Triangle Demo objects
        vkDestroyFence(_device, _f_frame_ready, nullptr);
        vkDestroySemaphore(_device, _s_render_start, nullptr);
        vkDestroySemaphore(_device, _s_render_done, nullptr);
        vkDestroyCommandPool(_device, _command_pool, nullptr);
        for (VkFramebuffer framebuffer : _framebuffers) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        vkDestroyPipeline(_device, _pipeline, nullptr);
        vkDestroyPipelineLayout(_device, _layout, nullptr);
        vkDestroyRenderPass(_device, _renderpass, nullptr);
        _vertex.destroy();
        _fragment.destroy();

        // Cleanup
        _swapchain.destroy();
        vkDestroyDevice(_device, nullptr);
        vkDestroySurfaceKHR(_instance, _surface, nullptr);
#ifdef DYN_DEBUG
        vkDestroyDebugUtilsMessengerEXT(_instance, _debugger, nullptr);
#endif
        vkDestroyInstance(_instance, nullptr);
    }

    void Renderer::refresh() {
        vkWaitForFences(_device, 1, &_f_frame_ready, VK_TRUE, UINT64_MAX);
        vkResetFences(_device, 1, &_f_frame_ready);

        VkQueue queue = VkDevice_queue(_device, _physical.graphics_queues, 0);

        unsigned image_index;
        VkResult_log("Acquire Swapchain Image",
                     vkAcquireNextImageKHR(_device,
                                           _swapchain.handle,
                                           UINT64_MAX,
                                           _s_render_start,
                                           VK_NULL_HANDLE,
                                           &image_index));

        VkResult_log("Reset Command Buffer", vkResetCommandBuffer(_command_buffer, 0));

        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = 0;
        begin_info.pInheritanceInfo = nullptr;

        VkResult_log("Begin Command Recording", vkBeginCommandBuffer(_command_buffer, &begin_info));

        VkRenderPassBeginInfo renderpass_begin_info = {};
        VkClearValue clear_value = {{{0, 0, 0, 1}}};
        renderpass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderpass_begin_info.renderPass = _renderpass;
        renderpass_begin_info.renderArea.extent = _swapchain.extent;
        renderpass_begin_info.renderArea.offset.x = 0;
        renderpass_begin_info.renderArea.offset.y = 0;
        renderpass_begin_info.clearValueCount = 1;
        renderpass_begin_info.pClearValues = &clear_value;
        renderpass_begin_info.framebuffer = _framebuffers[image_index];

        vkCmdBeginRenderPass(_command_buffer, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);

        VkViewport viewport;
        viewport.minDepth = 0;
        viewport.maxDepth = 1;
        viewport.width = _swapchain.extent.width;
        viewport.height = _swapchain.extent.height;
        viewport.x = 0;
        viewport.y = 0;
        vkCmdSetViewport(_command_buffer, 0, 1, &viewport);

        VkRect2D scissor;
        scissor.extent = _swapchain.extent;
        scissor.offset.x = 0;
        scissor.offset.y = 0;
        vkCmdSetScissor(_command_buffer, 0, 1, &scissor);

        vkCmdDraw(_command_buffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(_command_buffer);
        VkResult_log("End Command Buffer", vkEndCommandBuffer(_command_buffer));

        VkSubmitInfo submit_info = {};
        VkPipelineStageFlags wait_stage_mask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_command_buffer;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &_s_render_start;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &_s_render_done;
        submit_info.pWaitDstStageMask = &wait_stage_mask;

        VkResult_log("Graphics Submit", vkQueueSubmit(queue, 1, &submit_info, _f_frame_ready));

        VkPresentInfoKHR present_info = {};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &_s_render_done;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &_swapchain.handle;
        present_info.pImageIndices = &image_index;
        present_info.pResults = nullptr;

        VkResult_log("Graphics Present", vkQueuePresentKHR(queue, &present_info));
    }
} // namespace Dynamo::Graphics::Vulkan