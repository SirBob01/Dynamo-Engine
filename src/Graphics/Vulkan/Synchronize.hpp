#pragma once

#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Build a Vulkan fence.
     *
     * @param device
     * @return VkFence
     */
    VkFence VkFence_build(VkDevice device);

    /**
     * @brief Build a Vulkan semaphore.
     *
     * @param device
     * @return VkSemaphore
     */
    VkSemaphore VkSemaphore_build(VkDevice device);
} // namespace Dynamo::Graphics::Vulkan