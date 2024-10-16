#pragma once

#include <unordered_map>

#include <vulkan/vulkan_core.h>

#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Framebuffer cache.
     *
     */
    class FramebufferCache {
        VkDevice _device;
        std::unordered_map<FramebufferSettings, VkFramebuffer> _cache;

      public:
        FramebufferCache(VkDevice device);
        FramebufferCache() = default;

        /**
         * @brief Build a framebuffer.
         *
         * @param settings
         * @return VkFramebuffer
         */
        VkFramebuffer build(const FramebufferSettings &settings);

        /**
         * @brief Destroy all framebuffers, invalidating existing handles.
         *
         */
        void destroy();
    };
} // namespace Dynamo::Graphics::Vulkan