#include <Graphics/Vulkan/ImageView.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    ImageView::ImageView(Image &image, const ImageViewSettings &settings) :
        _image(image) {
        VkImageViewCreateInfo image_view_info = {};
        image_view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_info.image = image.handle();
        image_view_info.format = image.format();
        image_view_info.viewType = settings.type;
        image_view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        image_view_info.subresourceRange.aspectMask = settings.aspect_mask;
        image_view_info.subresourceRange.baseMipLevel = settings.mip_base;
        image_view_info.subresourceRange.levelCount = settings.mip_count;
        image_view_info.subresourceRange.baseArrayLayer = settings.layer_base;
        image_view_info.subresourceRange.layerCount = settings.layer_count;

        VkResult result = vkCreateImageView(_image.device().handle(),
                                            &image_view_info,
                                            nullptr,
                                            &_handle);
        if (result != VK_SUCCESS) {
            Log::error("Unable to create Vulkan::ImageView.");
        }
    }

    ImageView::~ImageView() {
        vkDestroyImageView(_image.device().handle(), _handle, nullptr);
    }

    VkImageView ImageView::handle() const { return _handle; }

    const Image &ImageView::image() const { return _image; }
} // namespace Dynamo::Graphics::Vulkan