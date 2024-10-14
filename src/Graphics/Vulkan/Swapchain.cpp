#include <Graphics/Vulkan/Swapchain.hpp>
#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    VkExtent2D compute_extent(const Display &display, const SwapchainOptions &options) {
        Vec2 size = display.get_framebuffer_size();
        VkExtent2D extent;
        extent.width = std::clamp(static_cast<unsigned>(size.x),
                                  options.capabilities.minImageExtent.width,
                                  options.capabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<unsigned>(size.y),
                                   options.capabilities.minImageExtent.height,
                                   options.capabilities.maxImageExtent.height);
        return extent;
    }

    VkSurfaceFormatKHR select_surface_format(const SwapchainOptions &options) {
        for (VkSurfaceFormatKHR query : options.formats) {
            if (query.format == VK_FORMAT_B8G8R8A8_SRGB && query.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return query;
            }
        }
        return options.formats[0];
    }

    VkPresentModeKHR select_present_mode(const Display &display, const SwapchainOptions &options) {
        VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
        for (VkPresentModeKHR query : options.present_modes) {
            if (!display.is_vsync() && query == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                present_mode = query;
            } else if (query == VK_PRESENT_MODE_MAILBOX_KHR) {
                present_mode = query;
            }
        }
        return present_mode;
    }

    Swapchain::Swapchain(VkDevice device,
                         const PhysicalDevice &physical,
                         const Display &display,
                         std::optional<VkSwapchainKHR> previous) {
        SwapchainOptions options = physical.get_swapchain_options();
        extent = compute_extent(display, options);
        surface_format = select_surface_format(options);
        present_mode = select_present_mode(display, options);

        // Create swapchain handle
        VkSwapchainCreateInfoKHR swapchain_info = {};
        swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchain_info.surface = physical.surface;
        swapchain_info.preTransform = options.capabilities.currentTransform;
        swapchain_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_info.presentMode = present_mode;
        swapchain_info.clipped = VK_TRUE;
        swapchain_info.imageFormat = surface_format.format;
        swapchain_info.imageColorSpace = surface_format.colorSpace;
        swapchain_info.imageExtent = extent;
        swapchain_info.imageArrayLayers = 1;
        swapchain_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        swapchain_info.minImageCount = options.capabilities.minImageCount + 1;
        if (options.capabilities.maxImageCount) {
            swapchain_info.minImageCount = options.capabilities.maxImageCount;
        }

        // Share images across graphics and present queue families
        std::array<unsigned, 2> queue_family_indices = {
            physical.graphics_queues.index,
            physical.present_queues.index,
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
        if (previous.has_value()) {
            swapchain_info.oldSwapchain = previous.value();
        }

        VkResult_log("Create Swapchain", vkCreateSwapchainKHR(device, &swapchain_info, nullptr, &handle));

        // Get swapchain images
        unsigned count = 0;
        vkGetSwapchainImagesKHR(device, handle, &count, nullptr);
        images.resize(count);
        vkGetSwapchainImagesKHR(device, handle, &count, images.data());
    }
} // namespace Dynamo::Graphics::Vulkan