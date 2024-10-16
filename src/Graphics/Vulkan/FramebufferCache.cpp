#include <Graphics/Vulkan/FramebufferCache.hpp>
#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    FramebufferCache::FramebufferCache(VkDevice device) : _device(device) {}

    VkFramebuffer FramebufferCache::build(const FramebufferSettings &settings) {
        auto framebuffer_it = _cache.find(settings);
        if (framebuffer_it != _cache.end()) {
            return framebuffer_it->second;
        }

        VkFramebuffer framebuffer = VkFramebuffer_create(_device, settings);
        _cache.emplace(settings, framebuffer);
        return framebuffer;
    }

    void FramebufferCache::destroy() {
        for (const auto [key, framebuffer] : _cache) {
            vkDestroyFramebuffer(_device, framebuffer, nullptr);
        }
        _cache.clear();
    }
} // namespace Dynamo::Graphics::Vulkan