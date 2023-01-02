#include "./VkImage.hpp"

namespace Dynamo::Graphics {
    VkImage::VkImage(vk::Device &device,
                     unsigned width,
                     unsigned height,
                     unsigned mip_levels,
                     vk::Format format,
                     vk::ImageTiling tiling,
                     vk::Flags<vk::ImageUsageFlagBits> usage,
                     vk::SampleCountFlagBits samples) :
        _device(device) {
        vk::ImageCreateInfo image_info;
        image_info.imageType = vk::ImageType::e2D;
        image_info.format = format;

        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = 1;

        image_info.mipLevels = mip_levels;
        image_info.arrayLayers = 1;
        image_info.samples = samples;

        image_info.tiling = tiling;
        image_info.usage = usage;
        image_info.sharingMode = vk::SharingMode::eExclusive;

        _handle = _device.get().createImageUnique(image_info);
    }

    const vk::Image &VkImage::get_handle() const { return *_handle; }

    vk::UniqueImageView
    VkImage::create_view(vk::Format format,
                         vk::Flags<vk::ImageAspectFlagBits> aspect_mask,
                         unsigned mip_levels) {
        vk::ImageViewCreateInfo view_info;
        view_info.image = *_handle;
        view_info.viewType = vk::ImageViewType::e2D;
        view_info.format = format;

        view_info.subresourceRange.aspectMask = aspect_mask;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = mip_levels;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        return _device.get().createImageViewUnique(view_info);
    }
} // namespace Dynamo::Graphics