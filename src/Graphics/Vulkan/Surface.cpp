#include <Graphics/Vulkan/Surface.hpp>

namespace Dynamo::Graphics::Vulkan {
    Surface::Surface(Instance &instance, const Display &display) :
        _instance(instance) {
        _handle = display.create_vulkan_surface(_instance.handle());
    }

    Surface::~Surface() {
        vkDestroySurfaceKHR(_instance.handle(), _handle, nullptr);
    }

    VkSurfaceKHR Surface::handle() const { return _handle; }
} // namespace Dynamo::Graphics::Vulkan