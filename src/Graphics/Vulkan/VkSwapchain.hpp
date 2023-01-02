#pragma once

#include <array>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Core/Display.hpp"
#include "./VkPhysical.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Create a vk::UniqueImage instance
     *
     * @param logical
     * @param width
     * @param height
     * @param mip_levels
     * @param format
     * @param tiling
     * @param usage
     * @param samples
     * @return vk::UniqueImage
     */
    vk::UniqueImage create_vk_image(vk::Device &logical,
                                    unsigned width,
                                    unsigned height,
                                    unsigned mip_levels,
                                    vk::Format format,
                                    vk::ImageTiling tiling,
                                    vk::Flags<vk::ImageUsageFlagBits> usage,
                                    vk::SampleCountFlagBits samples);

    /**
     * @brief Create a vk::UniqueImageView instance from an image
     *
     * @param logical
     * @param image
     * @param format
     * @param aspect_mask
     * @param mip_levels
     * @return vk::UniqueImageView
     */
    vk::UniqueImageView
    create_vk_image_view(vk::Device &logical,
                         vk::Image &image,
                         vk::Format format,
                         vk::Flags<vk::ImageAspectFlagBits> aspect_mask,
                         unsigned mip_levels);

    /**
     * @brief Wrapper class for a Vulkan swapchain
     *
     */
    class VkSwapchain {
        /**
         * @brief Swapchain handle
         *
         */
        vk::UniqueSwapchainKHR _handle;

        /**
         * @brief Swapchain images and their views
         *
         */
        std::vector<vk::Image> _images;
        std::vector<vk::UniqueImageView> _views;

        /**
         * @brief Extent of the swapchain (dimensions of the drawable area)
         *
         */
        vk::Extent2D _extent;

        /**
         * @brief Color format and color space
         *
         */
        vk::SurfaceFormatKHR _format;

        /**
         * @brief Presentation mode
         *
         */
        vk::PresentModeKHR _present_mode;

        /**
         * @brief Calculate the extent of the swapchain
         *
         * @param capabilities
         * @param display
         */
        void calculate_extent(const vk::SurfaceCapabilitiesKHR &capabilities,
                              Display &display);

        /**
         * @brief Select the surface format
         *
         * @param formats
         */
        void select_format(const std::vector<vk::SurfaceFormatKHR> &formats);

        /**
         * @brief Set the presentation mode of the swapchain
         *
         * @param present_modes
         * @param display
         */
        void set_presentation_mode(
            const std::vector<vk::PresentModeKHR> &present_modes,
            Display &display);

      public:
        /**
         * @brief Construct a new VkSwapchain object
         *
         * @param display
         * @param physical
         * @param logical
         * @param surface
         */
        VkSwapchain(Display &display,
                    VkPhysical &physical,
                    vk::Device &logical,
                    vk::SurfaceKHR &surface);

        /**
         * @brief Get the handle to the vk::SwapchainKHR
         *
         * @return const vk::SwapchainKHR&
         */
        const vk::SwapchainKHR &get_handle() const;

        /**
         * @brief Get the array of presentable images
         *
         * @return const std::vector<vk::Image>&
         */
        const std::vector<vk::Image> &get_images() const;

        /**
         * @brief Get the array of image views, which map one-to-one with the
         * presentable images array
         *
         * @return const std::vector<vk::UniqueImageView>&
         */
        const std::vector<vk::UniqueImageView> &get_views() const;

        /**
         * @brief Get the extent of the swapchain
         *
         * @return const vk::Extent2D&
         */
        const vk::Extent2D &get_extent() const;

        /**
         * @brief Get the surface format of the swapchain
         *
         * @return const vk::SurfaceFormatKHR&
         */
        const vk::SurfaceFormatKHR &get_format() const;

        /**
         * @brief Get the present mode of the swapchain
         *
         * @return const vk::PresentModeKHR&
         */
        const vk::PresentModeKHR &get_present_mode() const;
    };
} // namespace Dynamo::Graphics