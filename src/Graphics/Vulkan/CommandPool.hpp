#pragma once

#include <vector>

#include <vulkan/vulkan_core.h>

#include <Graphics/Vulkan/PhysicalDevice.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Build a Vulkan command pool.
     *
     * @param device
     * @param family
     * @return VkCommandPool
     */
    VkCommandPool VkCommandPool_build(VkDevice device, QueueFamily family);

    /**
     * @brief Allocate Vulkan command buffers from a pool.
     *
     * @param device
     * @param pool
     * @param level
     * @param count
     * @return std::vector<VkCommandBuffer>
     */
    std::vector<VkCommandBuffer>
    VkCommandBuffer_allocate(VkDevice device, VkCommandPool pool, VkCommandBufferLevel level, unsigned count);
} // namespace Dynamo::Graphics::Vulkan