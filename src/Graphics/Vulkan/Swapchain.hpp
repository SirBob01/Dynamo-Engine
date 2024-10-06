#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include <Graphics/Vulkan/Device.hpp>
#include <Graphics/Vulkan/Image.hpp>
#include <Graphics/Vulkan/ImageView.hpp>

namespace Dynamo::Graphics::Vulkan {
    class Swapchain {
        Device &_device;
        VkSwapchainKHR _handle;

        VkExtent2D _extent;
        VkSurfaceFormatKHR _format;
        VkPresentModeKHR _present_mode;

        std::vector<std::unique_ptr<Image>> _images;
        std::vector<std::unique_ptr<ImageView>> _image_views;

      public:
        Swapchain(Device &device,
                  Surface &surface,
                  const Display &display,
                  std::optional<VkSwapchainKHR> prev = {});
        ~Swapchain();

        const std::vector<std::unique_ptr<Image>> &images() const;

        const std::vector<std::unique_ptr<ImageView>> &image_views() const;
    };
} // namespace Dynamo::Graphics::Vulkan