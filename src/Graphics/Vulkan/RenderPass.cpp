#include "./RenderPass.hpp"

namespace Dynamo::Graphics::Vulkan {
    RenderPass::RenderPass(Device &device,
                           Swapchain &swapchain,
                           vk::RenderPassCreateInfo config) :
        _device(device),
        _swapchain(swapchain) {
        _handle = _device.get().get_handle().createRenderPass(config);

        // Create the framebuffers
        const vk::Extent2D extent = _swapchain.get().get_extent();
        for (const std::unique_ptr<ImageView> &target_view :
             _swapchain.get().get_views()) {
            std::array<vk::ImageView, 3> views = {
                _swapchain.get().get_color_view().get_handle(),
                _swapchain.get().get_depth_view().get_handle(),
                target_view->get_handle(),
            };

            vk::FramebufferCreateInfo fb_info;
            fb_info.renderPass = _handle;
            fb_info.attachmentCount = views.size();
            fb_info.pAttachments = views.data();
            fb_info.width = extent.width;
            fb_info.height = extent.height;
            fb_info.layers = 1;

            _framebuffers.push_back(
                _device.get().get_handle().createFramebuffer(fb_info));
        }
    }

    RenderPass::~RenderPass() {
        for (vk::Framebuffer &fb : _framebuffers) {
            _device.get().get_handle().destroyFramebuffer(fb);
        }
        _device.get().get_handle().destroyRenderPass(_handle);
    }

    const vk::RenderPass &RenderPass::get_handle() const { return _handle; }

    const std::vector<vk::Framebuffer> &RenderPass::get_framebuffers() const {
        return _framebuffers;
    }

    RenderPass create_base_2d_pass(Device &device, Swapchain &swapchain) {
        const PhysicalDevice &physical = device.get_physical();

        // Define attachment references to allow subpasses to reuse attachments
        vk::AttachmentReference color_ref;
        color_ref.attachment = 0;
        color_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference resolve_ref;
        resolve_ref.attachment = 1;
        resolve_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        // Color attachment
        vk::AttachmentDescription color_attachment;
        color_attachment.format = swapchain.get_format().format;
        color_attachment.samples = physical.get_msaa_samples();
        color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout = vk::ImageLayout::eUndefined;
        color_attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

        // Resolve attachment
        vk::AttachmentDescription resolve_attachment;
        resolve_attachment.format = swapchain.get_format().format;
        resolve_attachment.samples = vk::SampleCountFlagBits::e1;
        resolve_attachment.loadOp = vk::AttachmentLoadOp::eDontCare;
        resolve_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        resolve_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        resolve_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        resolve_attachment.initialLayout = vk::ImageLayout::eUndefined;
        resolve_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        std::array<vk::AttachmentDescription, 3> attachments;
        attachments[color_ref.attachment] = color_attachment;
        attachments[resolve_ref.attachment] = resolve_attachment;

        // Create the subpass
        vk::SubpassDescription initial_subpass;
        initial_subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        initial_subpass.colorAttachmentCount = 1;
        initial_subpass.pColorAttachments = &color_ref;
        initial_subpass.pResolveAttachments = &resolve_ref;
        std::vector<vk::SubpassDescription> subpasses = {initial_subpass};

        vk::SubpassDependency subpass_dependency;
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput;
        subpass_dependency.dstStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput;
        subpass_dependency.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        subpass_dependency.dstAccessMask =
            vk::AccessFlagBits::eColorAttachmentWrite;
        std::vector<vk::SubpassDependency> dependencies = {subpass_dependency};

        // Create the render pass
        vk::RenderPassCreateInfo render_pass_info;
        render_pass_info.attachmentCount = attachments.size();
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = subpasses.size();
        render_pass_info.pSubpasses = subpasses.data();
        render_pass_info.dependencyCount = dependencies.size();
        render_pass_info.pDependencies = dependencies.data();

        return RenderPass(device, swapchain, render_pass_info);
    }

    RenderPass create_layer_2d_pass(Device &device, Swapchain &swapchain) {
        const PhysicalDevice &physical = device.get_physical();

        // Define attachment references to allow subpasses to reuse attachments
        vk::AttachmentReference color_ref;
        color_ref.attachment = 0;
        color_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference resolve_ref;
        resolve_ref.attachment = 1;
        resolve_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        // Color attachment
        vk::AttachmentDescription color_attachment;
        color_attachment.format = swapchain.get_format().format;
        color_attachment.samples = physical.get_msaa_samples();
        color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout = vk::ImageLayout::eUndefined;
        color_attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

        // Resolve attachment
        vk::AttachmentDescription resolve_attachment;
        resolve_attachment.format = swapchain.get_format().format;
        resolve_attachment.samples = vk::SampleCountFlagBits::e1;
        resolve_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        resolve_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        resolve_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        resolve_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        resolve_attachment.initialLayout = vk::ImageLayout::eUndefined;
        resolve_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        std::array<vk::AttachmentDescription, 3> attachments;
        attachments[color_ref.attachment] = color_attachment;
        attachments[resolve_ref.attachment] = resolve_attachment;

        // Create the subpass
        vk::SubpassDescription initial_subpass;
        initial_subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        initial_subpass.colorAttachmentCount = 1;
        initial_subpass.pColorAttachments = &color_ref;
        initial_subpass.pResolveAttachments = &resolve_ref;
        std::vector<vk::SubpassDescription> subpasses = {initial_subpass};

        vk::SubpassDependency subpass_dependency;
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput;
        subpass_dependency.dstStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput;
        subpass_dependency.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        subpass_dependency.dstAccessMask =
            vk::AccessFlagBits::eColorAttachmentWrite;
        std::vector<vk::SubpassDependency> dependencies = {subpass_dependency};

        // Create the render pass
        vk::RenderPassCreateInfo render_pass_info;
        render_pass_info.attachmentCount = attachments.size();
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = subpasses.size();
        render_pass_info.pSubpasses = subpasses.data();
        render_pass_info.dependencyCount = dependencies.size();
        render_pass_info.pDependencies = dependencies.data();

        return RenderPass(device, swapchain, render_pass_info);
    }

    RenderPass create_base_3d_pass(Device &device, Swapchain &swapchain) {
        const PhysicalDevice &physical = device.get_physical();

        // Define attachment references to allow subpasses to reuse attachments
        vk::AttachmentReference color_ref;
        color_ref.attachment = 0;
        color_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference depth_ref;
        depth_ref.attachment = 1;
        depth_ref.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentReference resolve_ref;
        resolve_ref.attachment = 2;
        resolve_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        // Color attachment
        vk::AttachmentDescription color_attachment;
        color_attachment.format = swapchain.get_format().format;
        color_attachment.samples = physical.get_msaa_samples();
        color_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout = vk::ImageLayout::eUndefined;
        color_attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

        // Depth attachment
        vk::AttachmentDescription depth_attachment;
        depth_attachment.format = physical.get_depth_format();
        depth_attachment.samples = physical.get_msaa_samples();
        depth_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        depth_attachment.storeOp = vk::AttachmentStoreOp::eDontCare;
        depth_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        depth_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        depth_attachment.initialLayout = vk::ImageLayout::eUndefined;
        depth_attachment.finalLayout =
            vk::ImageLayout::eDepthStencilAttachmentOptimal;

        // Resolve attachment
        vk::AttachmentDescription resolve_attachment;
        resolve_attachment.format = swapchain.get_format().format;
        resolve_attachment.samples = vk::SampleCountFlagBits::e1;
        resolve_attachment.loadOp = vk::AttachmentLoadOp::eDontCare;
        resolve_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        resolve_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        resolve_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        resolve_attachment.initialLayout = vk::ImageLayout::eUndefined;
        resolve_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        std::array<vk::AttachmentDescription, 3> attachments;
        attachments[color_ref.attachment] = color_attachment;
        attachments[depth_ref.attachment] = depth_attachment;
        attachments[resolve_ref.attachment] = resolve_attachment;

        // Create the subpass
        vk::SubpassDescription initial_subpass;
        initial_subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        initial_subpass.colorAttachmentCount = 1;
        initial_subpass.pColorAttachments = &color_ref;
        initial_subpass.pDepthStencilAttachment = &depth_ref;
        initial_subpass.pResolveAttachments = &resolve_ref;
        std::vector<vk::SubpassDescription> subpasses = {initial_subpass};

        vk::SubpassDependency subpass_dependency;
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput |
            vk::PipelineStageFlagBits::eEarlyFragmentTests;
        subpass_dependency.dstStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput |
            vk::PipelineStageFlagBits::eEarlyFragmentTests;
        subpass_dependency.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        subpass_dependency.dstAccessMask =
            vk::AccessFlagBits::eColorAttachmentWrite |
            vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        std::vector<vk::SubpassDependency> dependencies = {subpass_dependency};

        // Create the render pass
        vk::RenderPassCreateInfo render_pass_info;
        render_pass_info.attachmentCount = attachments.size();
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = subpasses.size();
        render_pass_info.pSubpasses = subpasses.data();
        render_pass_info.dependencyCount = dependencies.size();
        render_pass_info.pDependencies = dependencies.data();

        return RenderPass(device, swapchain, render_pass_info);
    }

    RenderPass create_layer_3d_pass(Device &device, Swapchain &swapchain) {
        const PhysicalDevice &physical = device.get_physical();

        // Define attachment references to allow subpasses to reuse attachments
        vk::AttachmentReference color_ref;
        color_ref.attachment = 0;
        color_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::AttachmentReference depth_ref;
        depth_ref.attachment = 1;
        depth_ref.layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        vk::AttachmentReference resolve_ref;
        resolve_ref.attachment = 2;
        resolve_ref.layout = vk::ImageLayout::eColorAttachmentOptimal;

        // Color attachment
        vk::AttachmentDescription color_attachment;
        color_attachment.format = swapchain.get_format().format;
        color_attachment.samples = physical.get_msaa_samples();
        color_attachment.loadOp = vk::AttachmentLoadOp::eLoad;
        color_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        color_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        color_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        color_attachment.initialLayout = vk::ImageLayout::eUndefined;
        color_attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

        // Depth attachment
        vk::AttachmentDescription depth_attachment;
        depth_attachment.format = physical.get_depth_format();
        depth_attachment.samples = physical.get_msaa_samples();
        depth_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        depth_attachment.storeOp = vk::AttachmentStoreOp::eDontCare;
        depth_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        depth_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        depth_attachment.initialLayout = vk::ImageLayout::eUndefined;
        depth_attachment.finalLayout =
            vk::ImageLayout::eDepthStencilAttachmentOptimal;

        // Resolve attachment
        vk::AttachmentDescription resolve_attachment;
        resolve_attachment.format = swapchain.get_format().format;
        resolve_attachment.samples = vk::SampleCountFlagBits::e1;
        resolve_attachment.loadOp = vk::AttachmentLoadOp::eClear;
        resolve_attachment.storeOp = vk::AttachmentStoreOp::eStore;
        resolve_attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        resolve_attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        resolve_attachment.initialLayout = vk::ImageLayout::eUndefined;
        resolve_attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        std::array<vk::AttachmentDescription, 3> attachments;
        attachments[color_ref.attachment] = color_attachment;
        attachments[depth_ref.attachment] = depth_attachment;
        attachments[resolve_ref.attachment] = resolve_attachment;

        // Create the subpass
        vk::SubpassDescription initial_subpass;
        initial_subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        initial_subpass.colorAttachmentCount = 1;
        initial_subpass.pColorAttachments = &color_ref;
        initial_subpass.pDepthStencilAttachment = &depth_ref;
        initial_subpass.pResolveAttachments = &resolve_ref;
        std::vector<vk::SubpassDescription> subpasses = {initial_subpass};

        vk::SubpassDependency subpass_dependency;
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;
        subpass_dependency.srcStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput |
            vk::PipelineStageFlagBits::eEarlyFragmentTests;
        subpass_dependency.dstStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput |
            vk::PipelineStageFlagBits::eEarlyFragmentTests;
        subpass_dependency.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        subpass_dependency.dstAccessMask =
            vk::AccessFlagBits::eColorAttachmentWrite |
            vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        std::vector<vk::SubpassDependency> dependencies = {subpass_dependency};

        // Create the render pass
        vk::RenderPassCreateInfo render_pass_info;
        render_pass_info.attachmentCount = attachments.size();
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = subpasses.size();
        render_pass_info.pSubpasses = subpasses.data();
        render_pass_info.dependencyCount = dependencies.size();
        render_pass_info.pDependencies = dependencies.data();

        return RenderPass(device, swapchain, render_pass_info);
    }
} // namespace Dynamo::Graphics::Vulkan