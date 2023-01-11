#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"
#include "./MemoryPool.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Abstract base wrapper class for a Vulkan image
     *
     */
    class Image {
      protected:
        vk::Image _handle;
        std::reference_wrapper<Device> _device;
        vk::Format _format;

      public:
        /**
         * @brief Construct a new Image object from an existing handle
         *
         * @param device Reference to the logical device
         * @param handle Vulkan image handle
         * @param format Pixel format
         */
        Image(Device &device, vk::Image handle, vk::Format format);

        /**
         * @brief Construct a new Image object with parameters
         *
         * @param device      Reference to the logical device
         * @param width       Width of the image
         * @param height      Height of the image
         * @param depth       Depth of the image (3D texture)
         * @param mip_levels  Number of mipmap levels
         * @param layer_count Number of array layers
         * @param format      Pixel format
         * @param type        1D, 2D, or 3D
         * @param tiling      Tiling mode of the texels in-memory
         * @param usage       Usage flags
         */
        Image(Device &device,
              unsigned width,
              unsigned height,
              unsigned depth,
              unsigned mip_levels,
              unsigned layer_count,
              vk::Format format,
              vk::ImageType type,
              vk::ImageTiling tiling,
              vk::Flags<vk::ImageUsageFlagBits> usage);
        virtual ~Image() = 0;

        /**
         * @brief Get the handle to vk::Image
         *
         * @return const vk::Image&
         */
        const vk::Image &get_handle() const;

        /**
         * @brief Get the device
         *
         * @return Device&
         */
        Device &get_device();

        /**
         * @brief Get the format object
         *
         * @return vk::Format
         */
        vk::Format get_format();

        /**
         * @brief Get the memory requirements for the image
         *
         * @return vk::MemoryRequirements
         */
        vk::MemoryRequirements get_memory_requirements();
    };
    inline Image::~Image() = default;

    /**
     * @brief User-created image
     *
     */
    class UserImage : public Image {
        MemoryBlock _block;

      public:
        /**
         * @brief Construct a new UserImage object
         *
         * @param device      Reference to the logical device
         * @param memory_pool Reference to the memory pool
         * @param width       Width of the image
         * @param height      Height of the image
         * @param depth       Depth of the image (3D texture)
         * @param mip_levels  Number of mipmap levels
         * @param layer_count Number of array layers
         * @param format      Pixel format
         * @param type        1D, 2D, or 3D
         * @param tiling      Tiling mode of the texels in-memory
         * @param usage       Usage flags
         */
        UserImage(Device &device,
                  MemoryPool &memory_pool,
                  unsigned width,
                  unsigned height,
                  unsigned depth,
                  unsigned mip_levels,
                  unsigned layer_count,
                  vk::Format format,
                  vk::ImageType type,
                  vk::ImageTiling tiling,
                  vk::Flags<vk::ImageUsageFlagBits> usage);

        /**
         * @brief Destroy the UserImage object
         *
         */
        ~UserImage();
    };

    /**
     * @brief Swapchain images are a special case because they are owned by the
     * Vulkan instance, and so they cannot be manually destroyed
     *
     */
    class SwapchainImage : public Image {
      public:
        /**
         * @brief Construct a new SwapchainImage object
         *
         * @param device Reference to the logical device
         * @param handle Vulkan image handle
         * @param format Pixel format
         */
        SwapchainImage(Device &device, vk::Image handle, vk::Format format);

        /**
         * @brief Destroy the SwapchainImage object (do nothing)
         *
         */
        ~SwapchainImage();
    };
} // namespace Dynamo::Graphics::Vulkan