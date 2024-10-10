#pragma once

#include <vulkan/vulkan_core.h>

#include <Display.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Image view configuration settings.
     *
     */
    struct ImageViewSettings {
        VkFormat format = VK_FORMAT_B8G8R8A8_SRGB;
        VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D;
        VkImageAspectFlags aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;

        unsigned mip_base = 0;
        unsigned mip_count = 1;

        unsigned layer_base = 0;
        unsigned layer_count = 1;
    };

    /**
     * @brief Build a Vulkan image view.
     *
     * @param device
     * @param image
     * @param settings
     * @return VkImageView
     */
    VkImageView VkImageView_build(VkDevice device, VkImage image, ImageViewSettings settings);
} // namespace Dynamo::Graphics::Vulkan