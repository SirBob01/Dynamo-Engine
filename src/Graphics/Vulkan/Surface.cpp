#include <Graphics/Vulkan/Surface.hpp>
#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    Surface::Surface(Instance &instance, const Display &display) :
        _instance(instance) {
        _handle = display.create_vulkan_surface(_instance.handle());
    }

    Surface::~Surface() {
        vkDestroySurfaceKHR(_instance.handle(), _handle, nullptr);
    }
} // namespace Dynamo::Graphics::Vulkan