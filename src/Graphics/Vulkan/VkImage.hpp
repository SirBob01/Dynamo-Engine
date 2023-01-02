#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

namespace Dynamo::Graphics {
    /**
     * @brief Wrapper class for a Vulkan image
     *
     */
    class VkImage {
        vk::UniqueImage _handle;
        std::reference_wrapper<vk::Device> _device;

      public:
        /**
         * @brief Construct a new VkImage object with set parameters
         *
         * @param device
         * @param width
         * @param height
         * @param mip_levels
         * @param format
         * @param tiling
         * @param usage
         * @param samples
         */
        VkImage(vk::Device &device,
                unsigned width,
                unsigned height,
                unsigned mip_levels,
                vk::Format format,
                vk::ImageTiling tiling,
                vk::Flags<vk::ImageUsageFlagBits> usage,
                vk::SampleCountFlagBits samples);

        /**
         * @brief Get the handle to vk::Image
         *
         * @return const vk::Image&
         */
        const vk::Image &get_handle() const;

        /**
         * @brief Create an image view
         *
         * @param format
         * @param aspect_mask
         * @param mip_levels
         * @return vk::UniqueImageView
         */
        vk::UniqueImageView
        create_view(vk::Format format,
                    vk::Flags<vk::ImageAspectFlagBits> aspect_mask,
                    unsigned mip_levels);
    };
} // namespace Dynamo::Graphics