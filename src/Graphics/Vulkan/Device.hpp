#pragma once

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Build the Vulkan logical device.
     *
     * @param physical
     * @return VkDevice
     */
    VkDevice VkDevice_build(const PhysicalDevice &physical);

    /**
     * @brief Grab a handle to a device queue.
     *
     * @param device
     * @param family
     * @param index
     * @return VkQueue
     */
    VkQueue
    VkDevice_queue(VkDevice device, const QueueFamily &family, unsigned index);
} // namespace Dynamo::Graphics::Vulkan