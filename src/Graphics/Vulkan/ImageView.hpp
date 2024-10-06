#pragma once

#include <vulkan/vulkan.hpp>

#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/Vulkan/Image.hpp>

namespace Dynamo::Graphics::Vulkan {
    struct ImageViewSettings {
        VkImageViewType type = VK_IMAGE_VIEW_TYPE_2D;
        VkImageAspectFlags aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;

        unsigned mip_base = 0;
        unsigned mip_count = 1;

        unsigned layer_base = 0;
        unsigned layer_count = 1;
    };

    class ImageView {
        Image &_image;
        VkImageView _handle;

      public:
        ImageView(Image &image, const ImageViewSettings &settings);
        ~ImageView();

        VkImageView handle() const;

        const Image &image() const;
    };
} // namespace Dynamo::Graphics::Vulkan