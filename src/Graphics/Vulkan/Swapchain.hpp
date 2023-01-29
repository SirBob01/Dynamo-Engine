#pragma once

#include <array>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Types.hpp"
#include "../../Core/Display.hpp"
#include "../../Log/Log.hpp"

#include "./Device.hpp"
#include "./Image.hpp"
#include "./ImageView.hpp"
#include "./MemoryPool.hpp"
#include "./Semaphore.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan swapchain
     *
     */
    class Swapchain {
        vk::SwapchainKHR _handle;
        std::reference_wrapper<Device> _device;

        std::vector<std::unique_ptr<SwapchainImage>> _images;
        std::vector<std::unique_ptr<ImageView>> _views;

        std::unique_ptr<UserImage> _color_buffer;
        std::unique_ptr<UserImage> _depth_buffer;

        std::unique_ptr<ImageView> _color_view;
        std::unique_ptr<ImageView> _depth_view;

        vk::Extent2D _extent;
        vk::SurfaceFormatKHR _format;
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

        /**
         * @brief Create the swapchain images and their views
         *
         * @param memory_pool
         */
        void create_images(MemoryPool &memory_pool);

      public:
        /**
         * @brief Construct a new Swapchain object
         *
         * @param device      Reference to the logical device
         * @param display     Reference to the display window
         * @param memory_pool Reference to the memory pool
         * @param surface     Vulkan surface handle
         */
        Swapchain(Device &device,
                  MemoryPool &memory_pool,
                  Display &display,
                  vk::SurfaceKHR surface);

        /**
         * @brief Destroy the Swapchain object
         *
         */
        ~Swapchain();

        /**
         * @brief Get the handle to the vk::SwapchainKHR
         *
         * @return const vk::SwapchainKHR&
         */
        const vk::SwapchainKHR &get_handle() const;

        /**
         * @brief Get the array of presentable images
         *
         * @return const std::vector<std::unique_ptr<SwapchainImage>>&
         */
        const std::vector<std::unique_ptr<SwapchainImage>> &get_images() const;

        /**
         * @brief Get the array of image views, which map one-to-one with the
         * presentable images array
         *
         * @return const std::vector<std::unique_ptr<ImageView>>&
         */
        const std::vector<std::unique_ptr<ImageView>> &get_views() const;

        /**
         * @brief Get the color buffer view
         *
         * @return const ImageView&
         */
        const ImageView &get_color_view() const;

        /**
         * @brief Get the depth buffer view
         *
         * @return const ImageView&
         */
        const ImageView &get_depth_view() const;

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

        /**
         * @brief Acquire the index to the next presentation target image
         *
         * @param semaphore Semaphore to signal
         * @return std::optional<u32>
         */
        std::optional<u32>
        get_presentation_image(const Semaphore &semaphore) const;
    };
} // namespace Dynamo::Graphics::Vulkan