#include <Graphics/Renderer.hpp>
#include <Graphics/Vulkan/Utils.hpp>

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
    }

    Renderer::~Renderer() {
        // Wait for device queues to finish processing
        vkDeviceWaitIdle(_device);

        // Cleanup
        _swapchain.destroy();
        vkDestroyDevice(_device, nullptr);
        vkDestroySurfaceKHR(_instance, _surface, nullptr);
#ifdef DYN_DEBUG
        vkDestroyDebugUtilsMessengerEXT(_instance, _debugger, nullptr);
#endif
        vkDestroyInstance(_instance, nullptr);
    }

    void Renderer::refresh() {}
} // namespace Dynamo::Graphics::Vulkan