#pragma once

#include <vulkan/vulkan_core.h>

#include <Display.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Build the Vulkan instance.
     *
     * @param display
     * @return VkInstance
     */
    VkInstance VkInstance_build(const Display &display);
} // namespace Dynamo::Graphics::Vulkan