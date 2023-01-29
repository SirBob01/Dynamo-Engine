#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "../../Types.hpp"
#include "./Device.hpp"
#include "./Image.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan image view
     *
     */
    class ImageView {
        vk::ImageView _handle;
        std::reference_wrapper<Image> _image;

      public:
        /**
         * @brief Construct a new ImageView object
         *
         * @param image       Reference to the main image
         * @param type        1D, 2D, or 3D
         * @param aspect_mask Aspect of the pixel format to access
         * @param mip_levels  Number of mipmap levels
         * @param layer_count Number of layers
         */
        ImageView(Image &image,
                  vk::ImageViewType type,
                  vk::ImageAspectFlags aspect_mask,
                  u32 mip_levels,
                  u32 layer_count = 1);

        /**
         * @brief Destroy the ImageView object
         *
         */
        ~ImageView();

        /**
         * @brief Get the handle to vk::ImageView
         *
         * @return const vk::ImageView&
         */
        const vk::ImageView &get_handle() const;

        /**
         * @brief Get a reference to the main image
         *
         * @return Image&
         */
        Image &get_image();
    };
} // namespace Dynamo::Graphics::Vulkan