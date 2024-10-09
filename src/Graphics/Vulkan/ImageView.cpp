#include <Graphics/Vulkan/ImageView.hpp>
#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    VkImageView VkImageView_build(VkDevice device,
                                  VkImage image,
                                  ImageViewSettings settings) {
        VkImageViewCreateInfo view_info = {};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = image;
        view_info.format = settings.format;
        view_info.viewType = settings.type;
        view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        view_info.subresourceRange.aspectMask = settings.aspect_mask;
        view_info.subresourceRange.baseMipLevel = settings.mip_base;
        view_info.subresourceRange.levelCount = settings.mip_count;
        view_info.subresourceRange.baseArrayLayer = settings.layer_base;
        view_info.subresourceRange.layerCount = settings.layer_count;

        VkImageView view;
        VkResult_log("Create ImageView",
                     vkCreateImageView(device, &view_info, nullptr, &view));
        return view;
    }
} // namespace Dynamo::Graphics::Vulkan