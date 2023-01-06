#include "./Image.hpp"

namespace Dynamo::Graphics::Vulkan {
    Image::Image(Device &device, vk::Image handle, vk::Format format) :
        _handle(handle), _device(device), _format(format) {}

    Image::Image(Device &device,
                 unsigned width,
                 unsigned height,
                 unsigned depth,
                 unsigned mip_levels,
                 unsigned layer_count,
                 vk::Format format,
                 vk::ImageType type,
                 vk::ImageTiling tiling,
                 vk::Flags<vk::ImageUsageFlagBits> usage) :
        _device(device) {
        vk::ImageCreateInfo image_info;
        image_info.imageType = type;
        image_info.format = format;

        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = depth;

        image_info.mipLevels = mip_levels;
        image_info.arrayLayers = layer_count;
        image_info.samples = device.get_physical().get_msaa_samples();

        image_info.tiling = tiling;
        image_info.usage = usage;
        image_info.sharingMode = vk::SharingMode::eExclusive;

        _handle = device.get_handle().createImage(image_info);
        _format = format;
    }

    const vk::Image &Image::get_handle() const { return _handle; }

    Device &Image::get_device() { return _device.get(); }

    vk::Format Image::get_format() { return _format; }

    vk::MemoryRequirements Image::get_memory_requirements() {
        return _device.get().get_handle().getImageMemoryRequirements(_handle);
    }

    UserImage::UserImage(Device &device,
                         Allocator &allocator,
                         unsigned width,
                         unsigned height,
                         unsigned depth,
                         unsigned mip_levels,
                         unsigned layer_count,
                         vk::Format format,
                         vk::ImageType type,
                         vk::ImageTiling tiling,
                         vk::Flags<vk::ImageUsageFlagBits> usage) :
        Image(device,
              width,
              height,
              depth,
              mip_levels,
              layer_count,
              format,
              type,
              tiling,
              usage) {
        std::optional<Allocation> result =
            allocator.allocate(get_memory_requirements(),
                               vk::MemoryPropertyFlagBits::eDeviceLocal);
        if (!result.has_value()) {
            Log::error("Unable to allocate memory for a Vulkan image.");
        }

        _allocation = std::make_unique<Allocation>(result.value());
        _device.get().get_handle().bindImageMemory(
            _handle,
            _allocation->memory.get().get_handle(),
            _allocation->offset);
    }

    UserImage::~UserImage() {
        _device.get().get_handle().destroyImage(_handle);
        _allocation->memory.get().free(_allocation->offset);
    }

    SwapchainImage::SwapchainImage(Device &device,
                                   vk::Image handle,
                                   vk::Format format) :
        Image(device, handle, format) {}

    SwapchainImage::~SwapchainImage() {}
} // namespace Dynamo::Graphics::Vulkan