#include <Graphics/Vulkan/ImageView.hpp>
#include <Graphics/Vulkan/Swapchain.hpp>
#include <Math/Vec2.hpp>
#include <Utils/Log.hpp>

namespace Dynamo::Graphics::Vulkan {
    Swapchain::Swapchain(Device &device,
                         Surface &surface,
                         const Display &display,
                         std::optional<VkSwapchainKHR> prev) :
        _device(device) {
        const SwapchainOptions &options = device.physical().swapchain_options();

        // Calculate the extent
        Vec2 size = display.get_framebuffer_size();
        _extent.width = std::clamp(static_cast<unsigned>(size.x),
                                   options.capabilities.minImageExtent.width,
                                   options.capabilities.maxImageExtent.width);
        _extent.height = std::clamp(static_cast<unsigned>(size.y),
                                    options.capabilities.minImageExtent.height,
                                    options.capabilities.maxImageExtent.height);

        // Choose a surface format
        _format = options.formats[0];
        for (VkSurfaceFormatKHR format : options.formats) {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                _format = format;
                break;
            }
        }

        // Choose a present mode
        _present_mode = VK_PRESENT_MODE_FIFO_KHR;
        for (VkPresentModeKHR mode : options.present_modes) {
            if (!display.is_vsync() && mode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                _present_mode = mode;
            } else if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                _present_mode = mode;
            }
        }

        VkSwapchainCreateInfoKHR swapchain_info = {};
        swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_info.surface = surface.handle();
        swapchain_info.preTransform = options.capabilities.currentTransform;
        swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_info.presentMode = _present_mode;
        swapchain_info.clipped = VK_TRUE;
        swapchain_info.imageFormat = _format.format;
        swapchain_info.imageColorSpace = _format.colorSpace;
        swapchain_info.imageExtent = _extent;
        swapchain_info.imageArrayLayers = 1;
        swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_info.minImageCount = options.capabilities.minImageCount + 1;
        if (options.capabilities.maxImageCount) {
            swapchain_info.minImageCount = options.capabilities.maxImageCount;
        }

        // Share images across graphics and present queue families
        std::array<unsigned, 2> queue_family_indices = {
            _device.physical().graphics_queues().index,
            _device.physical().present_queues().index,
        };
        if (queue_family_indices[0] != queue_family_indices[1]) {
            swapchain_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchain_info.queueFamilyIndexCount = queue_family_indices.size();
            swapchain_info.pQueueFamilyIndices = queue_family_indices.data();
        } else {
            swapchain_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchain_info.queueFamilyIndexCount = 0;
            swapchain_info.pQueueFamilyIndices = nullptr;
        }

        // Handle swapchain recreation
        swapchain_info.oldSwapchain = VK_NULL_HANDLE;
        if (prev.has_value()) {
            swapchain_info.oldSwapchain = prev.value();
        }

        VkResult result = vkCreateSwapchainKHR(_device.handle(),
                                               &swapchain_info,
                                               nullptr,
                                               &_handle);
        if (result != VK_SUCCESS) {
            Log::error("Unable to create Vulkan::Swapchain.");
        }

        // Enumerate swapchain images
        unsigned image_count = 0;
        vkGetSwapchainImagesKHR(_device.handle(),
                                _handle,
                                &image_count,
                                nullptr);

        std::vector<VkImage> image_handles(image_count);
        vkGetSwapchainImagesKHR(_device.handle(),
                                _handle,
                                &image_count,
                                image_handles.data());

        for (VkImage handle : image_handles) {
            _images.emplace_back(
                std::make_unique<Image>(_device, handle, _format.format));
        }

        // Create swapchain image views
        for (std::unique_ptr<Image> &image : _images) {
            ImageViewSettings settings;
            _image_views.emplace_back(
                std::make_unique<ImageView>(*image, settings));
        }
    }

    Swapchain::~Swapchain() {
        vkDestroySwapchainKHR(_device.handle(), _handle, nullptr);
    }

    const std::vector<std::unique_ptr<Image>> &Swapchain::images() const {
        return _images;
    }

    const std::vector<std::unique_ptr<ImageView>> &
    Swapchain::image_views() const {
        return _image_views;
    }
} // namespace Dynamo::Graphics::Vulkan