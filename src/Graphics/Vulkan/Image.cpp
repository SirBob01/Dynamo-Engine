#include "./Image.hpp"

namespace Dynamo::Graphics::Vulkan {
    Image::Image(Device &device,
                 unsigned width,
                 unsigned height,
                 unsigned mip_levels,
                 vk::Format format,
                 vk::ImageTiling tiling,
                 vk::Flags<vk::ImageUsageFlagBits> usage) :
        _device(device),
        _format(format) {
        vk::ImageCreateInfo image_info;
        image_info.imageType = vk::ImageType::e2D;
        image_info.format = _format;

        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = 1;

        image_info.mipLevels = mip_levels;
        image_info.arrayLayers = 1;
        image_info.samples = device.get_physical().get_msaa_samples();

        image_info.tiling = tiling;
        image_info.usage = usage;
        image_info.sharingMode = vk::SharingMode::eExclusive;

        _handle = _device.get().get_handle().createImageUnique(image_info);
    }

    const vk::Image &Image::get_handle() const { return *_handle; }

    vk::UniqueImageView
    Image::create_view(vk::Flags<vk::ImageAspectFlagBits> aspect_mask,
                       unsigned mip_levels) const {
        vk::ImageViewCreateInfo view_info;
        view_info.image = *_handle;
        view_info.viewType = vk::ImageViewType::e2D;
        view_info.format = _format;

        view_info.subresourceRange.aspectMask = aspect_mask;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = mip_levels;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        return _device.get().get_handle().createImageViewUnique(view_info);
    }

    vk::MemoryRequirements Image::get_memory_requirements() {
        return _device.get().get_handle().getImageMemoryRequirements(*_handle);
    }
} // namespace Dynamo::Graphics::Vulkan