#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan image
     *
     */
    class Image {
        vk::UniqueImage _handle;
        std::reference_wrapper<Device> _device;

        /**
         * @brief Pixel format of the image
         *
         */
        vk::Format _format;

      public:
        /**
         * @brief Construct a new Image object with set parameters
         *
         * @param width
         * @param height
         * @param mip_levels
         * @param format
         * @param tiling
         * @param usage
         */
        Image(Device &device,
              unsigned width,
              unsigned height,
              unsigned mip_levels,
              vk::Format format,
              vk::ImageTiling tiling,
              vk::Flags<vk::ImageUsageFlagBits> usage);

        /**
         * @brief Get the handle to vk::Image
         *
         * @return const vk::Image&
         */
        const vk::Image &get_handle() const;

        /**
         * @brief Create a view for the image with the same pixel format
         *
         * @param aspect_mask
         * @param mip_levels
         * @return vk::UniqueImageView
         */
        vk::UniqueImageView
        create_view(vk::Flags<vk::ImageAspectFlagBits> aspect_mask,
                    unsigned mip_levels) const;

        /**
         * @brief Get the memory requirements for the image
         *
         * @return vk::MemoryRequirements
         */
        vk::MemoryRequirements get_memory_requirements();
    };
} // namespace Dynamo::Graphics::Vulkan