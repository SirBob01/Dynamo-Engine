#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    Swapchain::Swapchain(Device &device,
                         MemoryPool &memory_pool,
                         Display &display,
                         vk::SurfaceKHR surface) :
        _device(device) {
        PhysicalDevice &physical = _device.get().get_physical();
        const SwapchainOptions &options = physical.get_swapchain_options();
        calculate_extent(options.capabilities, display);
        select_format(options.formats);
        set_presentation_mode(options.present_modes, display);

        // Determines the number of images to be rendered to
        // This allows multiple buffering
        u32 image_count = options.capabilities.minImageCount + 1;
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
            physical.get_queue_properties(QueueFamily::Graphics);
        const QueueProperties &transfer_queue_properties =
            physical.get_queue_properties(QueueFamily::Transfer);
        const QueueProperties &present_queue_properties =
            physical.get_queue_properties(QueueFamily::Present);

        std::array<u32, 3> index_arr;
        index_arr[0] = present_queue_properties.family_id;
        u32 index_count = 1;
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

        // Create the swapchain and its images
        const vk::Device &logical = device.get_handle();
        _handle = _device.get().get_handle().createSwapchainKHR(swapchain_info);
        create_images(memory_pool);
    }

    Swapchain::~Swapchain() {
        _device.get().get_handle().destroySwapchainKHR(_handle);
    }

    void
    Swapchain::calculate_extent(const vk::SurfaceCapabilitiesKHR &capabilities,
                                Display &display) {
        i32 width, height;
        glfwGetFramebufferSize(display.get_window(), &width, &height);

        _extent.width = std::clamp(static_cast<u32>(width),
                                   capabilities.minImageExtent.width,
                                   capabilities.maxImageExtent.width);
        _extent.height = std::clamp(static_cast<u32>(height),
                                    capabilities.minImageExtent.height,
                                    capabilities.maxImageExtent.height);
    }

    void
    Swapchain::select_format(const std::vector<vk::SurfaceFormatKHR> &formats) {
        _format = formats[0];
        for (const vk::SurfaceFormatKHR &format : formats) {
            if (format.format == vk::Format::eB8G8R8A8Srgb &&
                format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                _format = format;
                return;
            }
        }
    }

    void Swapchain::set_presentation_mode(
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

    void Swapchain::create_images(MemoryPool &memory_pool) {
        vk::Device logical = _device.get().get_handle();
        PhysicalDevice &physical = _device.get().get_physical();

        for (vk::Image &vkimg : logical.getSwapchainImagesKHR(_handle)) {
            _images.emplace_back(
                std::make_unique<SwapchainImage>(_device.get(),
                                                 vkimg,
                                                 _format.format));
            _views.emplace_back(
                std::make_unique<ImageView>(*_images.back(),
                                            vk::ImageViewType::e2D,
                                            vk::ImageAspectFlagBits::eColor,
                                            1));
        }

        // Create the color buffer
        _color_buffer = std::make_unique<UserImage>(
            _device,
            memory_pool,
            _extent.width,
            _extent.height,
            1,
            1,
            1,
            _format.format,
            vk::ImageType::e2D,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eColorAttachment,
            physical.get_msaa_samples());
        _color_view =
            std::make_unique<ImageView>(*_color_buffer,
                                        vk::ImageViewType::e2D,
                                        vk::ImageAspectFlagBits::eColor,
                                        1);

        // Create the depth buffer
        _depth_buffer = std::make_unique<UserImage>(
            _device,
            memory_pool,
            _extent.width,
            _extent.height,
            1,
            1,
            1,
            physical.get_depth_format(),
            vk::ImageType::e2D,
            vk::ImageTiling::eOptimal,
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            physical.get_msaa_samples());
        _depth_view =
            std::make_unique<ImageView>(*_depth_buffer,
                                        vk::ImageViewType::e2D,
                                        vk::ImageAspectFlagBits::eDepth,
                                        1);
    }

    const vk::SwapchainKHR &Swapchain::get_handle() const { return _handle; }

    const std::vector<std::unique_ptr<SwapchainImage>> &
    Swapchain::get_images() const {
        return _images;
    }

    const std::vector<std::unique_ptr<ImageView>> &
    Swapchain::get_views() const {
        return _views;
    }

    const ImageView &Swapchain::get_color_view() const { return *_color_view; }

    const ImageView &Swapchain::get_depth_view() const { return *_depth_view; }

    const vk::Extent2D &Swapchain::get_extent() const { return _extent; }

    const vk::SurfaceFormatKHR &Swapchain::get_format() const {
        return _format;
    }

    const vk::PresentModeKHR &Swapchain::get_present_mode() const {
        return _present_mode;
    }

    std::optional<u32>
    Swapchain::get_presentation_image(const Semaphore &semaphore) const {
        vk::ResultValue<u32> acquired =
            _device.get().get_handle().acquireNextImageKHR(
                _handle,
                UINT64_MAX,
                semaphore.get_handle(),
                nullptr);

        switch (acquired.result) {
        case vk::Result::eSuccess:
            return acquired.value;
        case vk::Result::eSuboptimalKHR:
        case vk::Result::eErrorOutOfDateKHR:
            return {};
        default:
            Log::error("Could not acquire image from the Vulkan swapchain: {}",
                       vk::to_string(acquired.result));
            break;
        }
        return acquired.value;
    }
} // namespace Dynamo::Graphics::Vulkan