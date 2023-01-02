#include "./VkSwapchain.hpp"

namespace Dynamo::Graphics {
    vk::UniqueImage create_vk_image(vk::Device &logical,
                                    unsigned width,
                                    unsigned height,
                                    unsigned mip_levels,
                                    vk::Format format,
                                    vk::ImageTiling tiling,
                                    vk::Flags<vk::ImageUsageFlagBits> usage,
                                    vk::SampleCountFlagBits samples) {
        vk::ImageCreateInfo image_info;
        image_info.imageType = vk::ImageType::e2D;
        image_info.format = format;

        image_info.extent.width = width;
        image_info.extent.height = height;
        image_info.extent.depth = 1;

        image_info.mipLevels = mip_levels;
        image_info.arrayLayers = 1;
        image_info.samples = samples;

        image_info.tiling = tiling;
        image_info.usage = usage;
        image_info.sharingMode = vk::SharingMode::eExclusive;

        return logical.createImageUnique(image_info);
    }

    vk::UniqueImageView
    create_vk_image_view(vk::Device &logical,
                         vk::Image &image,
                         vk::Format format,
                         vk::Flags<vk::ImageAspectFlagBits> aspect_mask,
                         unsigned mip_levels) {
        vk::ImageViewCreateInfo view_info;
        view_info.image = image;
        view_info.viewType = vk::ImageViewType::e2D;
        view_info.format = format;

        view_info.subresourceRange.aspectMask = aspect_mask;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = mip_levels;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;

        return logical.createImageViewUnique(view_info);
    }

    VkSwapchain::VkSwapchain(Display &display,
                             VkPhysical &physical,
                             vk::Device &logical,
                             vk::SurfaceKHR &surface) {
        const SwapchainOptions &options = physical.get_swapchain_options();
        calculate_extent(options.capabilities, display);
        select_format(options.formats);
        set_presentation_mode(options.present_modes, display);

        // Determines the number of images to be rendered to
        // This allows multiple buffering
        unsigned image_count = options.capabilities.minImageCount + 1;
        if (options.capabilities.maxImageCount) {
            image_count =
                std::min(image_count, options.capabilities.maxImageCount);
        }

        // Define the swapchain
        vk::SwapchainCreateInfoKHR swapchain_info;
        swapchain_info.surface = surface;
        swapchain_info.minImageCount = image_count;
        swapchain_info.preTransform = options.capabilities.currentTransform;
        swapchain_info.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapchain_info.presentMode = _present_mode;
        swapchain_info.clipped = true;

        // Define shared properties for each swapchain image
        swapchain_info.imageFormat = _format.format;
        swapchain_info.imageColorSpace = _format.colorSpace;
        swapchain_info.imageExtent = _extent;
        swapchain_info.imageArrayLayers = 1;
        swapchain_info.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        swapchain_info.imageSharingMode = vk::SharingMode::eExclusive;

        // Allow multiple command queues to access the images concurrently
        const QueueProperties &graphics_queue_properties =
            physical.get_graphics_queue_properties();
        const QueueProperties &present_queue_properties =
            physical.get_present_queue_properties();
        const QueueProperties &transfer_queue_properties =
            physical.get_transfer_queue_properties();

        std::array<unsigned, 3> index_arr;
        index_arr[0] = present_queue_properties.family_id;
        unsigned index_count = 1;
        if (graphics_queue_properties.family_id !=
            present_queue_properties.family_id) {
            index_arr[index_count++] = graphics_queue_properties.family_id;
        }
        if (transfer_queue_properties.family_id !=
            present_queue_properties.family_id) {
            index_arr[index_count++] = transfer_queue_properties.family_id;
        }
        if (index_count > 1) {
            swapchain_info.imageSharingMode = vk::SharingMode::eConcurrent;
            swapchain_info.queueFamilyIndexCount = index_count;
            swapchain_info.pQueueFamilyIndices = index_arr.data();
        }

        // Create the swapchain, its images, and their views
        _handle = logical.createSwapchainKHRUnique(swapchain_info);
        _images = logical.getSwapchainImagesKHR(_handle.get());
        for (vk::Image &image : _images) {
            _views.push_back(
                create_vk_image_view(logical,
                                     image,
                                     _format.format,
                                     vk::ImageAspectFlagBits::eColor,
                                     1));
        }
    }

    void VkSwapchain::calculate_extent(
        const vk::SurfaceCapabilitiesKHR &capabilities,
        Display &display) {
        int width, height;
        glfwGetFramebufferSize(display.get_window(), &width, &height);

        _extent.width = std::clamp(static_cast<unsigned>(width),
                                   capabilities.minImageExtent.width,
                                   capabilities.maxImageExtent.width);
        _extent.height = std::clamp(static_cast<unsigned>(height),
                                    capabilities.minImageExtent.height,
                                    capabilities.maxImageExtent.height);
    }

    void VkSwapchain::select_format(
        const std::vector<vk::SurfaceFormatKHR> &formats) {
        _format = formats[0];
        for (const vk::SurfaceFormatKHR &format : formats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb &&
                format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                _format = format;
                return;
            }
        }
    }

    void VkSwapchain::set_presentation_mode(
        const std::vector<vk::PresentModeKHR> &present_modes,
        Display &display) {
        _present_mode = vk::PresentModeKHR::eFifo;
        for (const vk::PresentModeKHR &mode : present_modes) {
            if (!display.is_vsync() && mode == vk::PresentModeKHR::eImmediate) {
                _present_mode = mode;
            } else if (mode == vk::PresentModeKHR::eMailbox) {
                _present_mode = mode;
            }
        }
    }

    const vk::SwapchainKHR &VkSwapchain::get_handle() const { return *_handle; }

    const std::vector<vk::Image> &VkSwapchain::get_images() const {
        return _images;
    }

    const std::vector<vk::UniqueImageView> &VkSwapchain::get_views() const {
        return _views;
    }

    const vk::Extent2D &VkSwapchain::get_extent() const { return _extent; }

    const vk::SurfaceFormatKHR &VkSwapchain::get_format() const {
        return _format;
    }

    const vk::PresentModeKHR &VkSwapchain::get_present_mode() const {
        return _present_mode;
    }
} // namespace Dynamo::Graphics