#include "./Texture.hpp"

namespace Dynamo::Graphics::Vulkan {
    Texture::Texture(std::vector<u8> &pixels,
                     u32 width,
                     u32 height,
                     Device &device,
                     MemoryPool &memory_pool,
                     Buffer &staging_buffer,
                     CommandPool &command_pool,
                     vk::Queue graphics_queue) :
        Dynamo::Graphics::Texture(pixels, width, height),
        _mip_levels(std::floor(std::log2(std::max(width, height))) + 1),
        _image(device,
               memory_pool,
               width,
               height,
               1,
               _mip_levels,
               1,
               vk::Format::eR8G8B8A8Srgb,
               vk::ImageType::e2D,
               vk::ImageTiling::eOptimal,
               TEXTURE_IMAGE_USAGE_FLAGS,
               vk::SampleCountFlagBits::e1),
        _view(_image,
              vk::ImageViewType::e2D,
              vk::ImageAspectFlagBits::eColor,
              1),
        _device(device), _command_pool(command_pool) {
        _command_buffer = std::move(
            command_pool.allocate(vk::CommandBufferLevel::ePrimary, 1)[0]);
        _graphics_queue = graphics_queue;

        transition_layout(vk::ImageLayout::eUndefined,
                          vk::ImageLayout::eTransferDstOptimal);
        copy_pixels(pixels.data(), staging_buffer);
        generate_mipmaps();
    }

    void Texture::transition_layout(vk::ImageLayout src, vk::ImageLayout dst) {
        // Define the memory barrier
        vk::ImageMemoryBarrier barrier;
        barrier.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        barrier.dstAccessMask = vk::AccessFlagBits::eNoneKHR;
        barrier.oldLayout = src;
        barrier.newLayout = dst;
        barrier.srcQueueFamilyIndex = 0;
        barrier.dstQueueFamilyIndex = 0;
        barrier.image = _image.get_handle();

        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = _mip_levels;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        vk::PipelineStageFlags src_stage, dst_stage;
        if (src == vk::ImageLayout::eUndefined &&
            dst == vk::ImageLayout::eTransferDstOptimal) {
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
            src_stage = vk::PipelineStageFlagBits::eTopOfPipe;
            dst_stage = vk::PipelineStageFlagBits::eTransfer;
        } else if (src == vk::ImageLayout::eTransferDstOptimal &&
                   dst == vk::ImageLayout::eShaderReadOnlyOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
            src_stage = vk::PipelineStageFlagBits::eTransfer;
            dst_stage = vk::PipelineStageFlagBits::eFragmentShader;
        } else {
            Log::error("Texture loading failed, Vulkan layout transition from "
                       "{} to {} is unsupported.",
                       vk::to_string(src),
                       vk::to_string(dst));
        }

        // Reset command buffers
        _command_pool.get().reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        // Execute the transition command
        vk::CommandBufferBeginInfo begin_info;
        begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        _command_buffer->begin(begin_info);
        _command_buffer->pipelineBarrier(
            src_stage,
            dst_stage,
            vk::DependencyFlagBits::eByRegion, // TODO
            nullptr,
            nullptr,
            barrier);
        _command_buffer->end();

        // Submit the command to the graphics queue
        vk::SubmitInfo submit_info;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_command_buffer.get();

        _graphics_queue.submit(submit_info, nullptr);
        _graphics_queue.waitIdle();
    }

    void Texture::copy_pixels(u8 pixels[], Buffer &staging_buffer) {
        // Copy the pixel data to the staging buffer
        u32 image_size = _width * _height * 4;
        u32 offset = staging_buffer.reserve(image_size, 1);
        staging_buffer.write(pixels, offset, image_size);
        staging_buffer.free(offset);

        // Copy from the staging buffer to the image
        vk::BufferImageCopy copy_region;
        copy_region.imageSubresource.aspectMask =
            vk::ImageAspectFlagBits::eColor;
        copy_region.imageSubresource.mipLevel = 0;
        copy_region.imageSubresource.baseArrayLayer = 0;
        copy_region.imageSubresource.layerCount = 1;

        copy_region.imageExtent.width = _width;
        copy_region.imageExtent.height = _height;
        copy_region.imageExtent.depth = 1;

        // Reset command buffers
        _command_pool.get().reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        vk::CommandBufferBeginInfo begin_info;
        begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

        _command_buffer->begin(begin_info);
        _command_buffer->copyBufferToImage(staging_buffer.get_handle(),
                                           _image.get_handle(),
                                           vk::ImageLayout::eTransferDstOptimal,
                                           1,
                                           &copy_region);
        _command_buffer->end();

        // Submit to the queue
        vk::SubmitInfo submit_info;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_command_buffer.get();

        _graphics_queue.submit(submit_info, nullptr);
        _graphics_queue.waitIdle();
    }

    void Texture::generate_mipmaps() {
        auto format_properties =
            _device.get().get_physical().get_format_properties(
                vk::Format::eR8G8B8A8Srgb);
        auto tiling_features = format_properties.optimalTilingFeatures;
        if (!(tiling_features &
              vk::FormatFeatureFlagBits::eSampledImageFilterLinear)) {
            // Cannot perform linear filtering to generate mipmaps
            // Just transition straight to shader readable layout
            transition_layout(vk::ImageLayout::eTransferDstOptimal,
                              vk::ImageLayout::eShaderReadOnlyOptimal);
            return;
        }

        vk::ImageMemoryBarrier barrier;
        barrier.srcQueueFamilyIndex = 0;
        barrier.dstQueueFamilyIndex = 0;
        barrier.image = _image.get_handle();

        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        // Reset command buffers
        _command_pool.get().reset(
            vk::CommandPoolResetFlagBits::eReleaseResources);

        // Execute commands for blitting the mipped textures
        vk::CommandBufferBeginInfo begin_info;
        begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        _command_buffer->begin(begin_info);

        u32 mip_width = _width;
        u32 mip_height = _height;
        for (u32 i = 1; i < _mip_levels; i++) {
            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;

            _command_buffer->pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eTransfer,
                vk::DependencyFlagBits::eByRegion,
                nullptr,
                nullptr,
                barrier);

            // Blit the new mip
            vk::ImageBlit blit;
            blit.srcOffsets[0] = vk::Offset3D(0, 0, 0);
            blit.srcOffsets[1] = vk::Offset3D(mip_width, mip_height, 1);
            blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;

            blit.dstOffsets[0] = vk::Offset3D(0, 0, 0);
            blit.dstOffsets[1] =
                vk::Offset3D(mip_width > 1 ? mip_width / 2 : 1,
                             mip_height > 1 ? mip_height / 2 : 1,
                             1);
            blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            _command_buffer->blitImage(_image.get_handle(),
                                       vk::ImageLayout::eTransferSrcOptimal,
                                       _image.get_handle(),
                                       vk::ImageLayout::eTransferDstOptimal,
                                       blit,
                                       vk::Filter::eLinear);

            // Transition mip to shader readable format
            barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
            barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

            _command_buffer->pipelineBarrier(
                vk::PipelineStageFlagBits::eTransfer,
                vk::PipelineStageFlagBits::eFragmentShader,
                vk::DependencyFlagBits::eByRegion,
                nullptr,
                nullptr,
                barrier);

            if (mip_width > 1) mip_width /= 2;
            if (mip_height > 1) mip_height /= 2;
        }

        // Transition last mip to optimal shader readable layout
        barrier.subresourceRange.baseMipLevel = _mip_levels - 1;
        barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

        _command_buffer->pipelineBarrier(
            vk::PipelineStageFlagBits::eTransfer,
            vk::PipelineStageFlagBits::eFragmentShader,
            vk::DependencyFlagBits::eByRegion,
            nullptr,
            nullptr,
            barrier);
        _command_buffer->end();

        // Submit to the queue
        vk::SubmitInfo submit_info;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &_command_buffer.get();

        _graphics_queue.submit(submit_info, nullptr);
        _graphics_queue.waitIdle();
    }

    const Image &Texture::get_image() const { return _image; }

    const ImageView &Texture::get_view() const { return _view; }
} // namespace Dynamo::Graphics::Vulkan