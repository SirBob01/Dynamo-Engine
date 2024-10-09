#pragma once

#include <string>

#include <vulkan/vulkan_core.h>

#include <Display.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Convert VkPhysicalDeviceType to string.
     *
     * @param type
     * @return const char*
     */
    const char *VkPhysicalDeviceType_string(VkPhysicalDeviceType type);

    /**
     * @brief Convert VkResult to string.
     *
     * @param type
     * @return const char*
     */
    const char *VkResult_string(VkResult type);

    /**
     * @brief Log the result of a Vulkan operation.
     *
     * @param op_message
     * @param result
     */
    void VkResult_log(const std::string &op_message, VkResult result);
} // namespace Dynamo::Graphics::Vulkan