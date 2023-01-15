#include "./ImageView.hpp"

namespace Dynamo::Graphics::Vulkan {
    ImageView::ImageView(Image &image,
                         vk::ImageViewType type,
                         vk::ImageAspectFlags aspect_mask,
                         unsigned mip_levels,
                         unsigned layer_count) :
        _image(image) {
        vk::ImageViewCreateInfo view_info;
        view_info.image = image.get_handle();
        view_info.format = image.get_format();
        view_info.viewType = type;

        view_info.subresourceRange.aspectMask = aspect_mask;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = mip_levels;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = layer_count;

        _handle = image.get_device().get_handle().createImageView(view_info);
    }

    ImageView::~ImageView() {
        _image.get().get_device().get_handle().destroyImageView(_handle);
    }

    const vk::ImageView &ImageView::get_handle() const { return _handle; }

    Image &ImageView::get_image() { return _image.get(); }
} // namespace Dynamo::Graphics::Vulkan