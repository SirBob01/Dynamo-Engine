#pragma once

#include <vulkan/vulkan.hpp>

#include "../../../Types.hpp"
#include "./Buffer.hpp"
#include "./CommandPool.hpp"
#include "./Device.hpp"
#include "./Image.hpp"
#include "./ImageView.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Texture image usage flags.
     *
     */
    constexpr vk::ImageUsageFlags TEXTURE_IMAGE_USAGE_FLAGS =
        vk::ImageUsageFlagBits::eTransferSrc |
        vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;

    /**
     * @brief 2D Texture.
     *
     */
    class Texture {
        u32 _width;
        u32 _height;
        u32 _mip_levels;

        UserImage _image;
        ImageView _view;

        std::reference_wrapper<Device> _device;
        std::reference_wrapper<CommandPool> _command_pool;

        vk::UniqueCommandBuffer _command_buffer;
        vk::Queue _graphics_queue;

        /**
         * @brief Transition the texture image to a new layout.
         *
         * @param src
         * @param dst
         */
        void transition_layout(vk::ImageLayout src, vk::ImageLayout dst);

        /**
         * @brief Copy pixels to the image via a staging buffer.
         *
         * @param pixels
         * @param staging_buffer
         */
        void copy_pixels(u8 pixels[], Buffer &staging_buffer);

        /**
         * @brief Generate the image mip maps.
         *
         */
        void generate_mipmaps();

      public:
        Texture(u8 pixels[],
                u32 width,
                u32 height,
                Device &device,
                MemoryPool &memory_pool,
                Buffer &staging_buffer,
                CommandPool &command_pool,
                vk::Queue graphics_queue);

        /**
         * @brief Get the image.
         *
         * @return const Image&
         */
        const Image &get_image() const;

        /**
         * @brief Get the image view.
         *
         * @return const ImageView&
         */
        const ImageView &get_view() const;
    };
} // namespace Dynamo::Graphics::Vulkan