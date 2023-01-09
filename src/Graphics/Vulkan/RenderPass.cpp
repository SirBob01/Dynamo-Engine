#include "./RenderPass.hpp"

namespace Dynamo::Graphics::Vulkan {
    RenderPass::RenderPass(Device &device, Swapchain &swapchain) :
        _device(device), _swapchain(swapchain) {

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

        // Attachment set
        std::array<vk::AttachmentDescription, 3> attachments;
        attachments[color_ref.attachment] = create_color_attachment();
        attachments[depth_ref.attachment] = create_depth_attachment();
        attachments[resolve_ref.attachment] = create_resolve_attachment();

        // Subpasses and their dependences
        std::vector<vk::SubpassDescription> subpasses =
            create_subpasses(color_ref, depth_ref, resolve_ref);
        std::vector<vk::SubpassDependency> dependencies =
            create_subpass_dependencies();

        // Create the render pass
        vk::RenderPassCreateInfo render_pass_info;
        render_pass_info.attachmentCount = attachments.size();
        render_pass_info.pAttachments = attachments.data();
        render_pass_info.subpassCount = subpasses.size();
        render_pass_info.pSubpasses = subpasses.data();
        render_pass_info.dependencyCount = dependencies.size();
        render_pass_info.pDependencies = dependencies.data();

        _handle = _device.get().get_handle().createRenderPass(render_pass_info);
    }

    RenderPass::~RenderPass() {
        _device.get().get_handle().destroyRenderPass(_handle);
    }

    vk::AttachmentDescription RenderPass::create_color_attachment() {
        const PhysicalDevice &physical = _device.get().get_physical();
        vk::AttachmentDescription attachment;
        attachment.format = _swapchain.get().get_format().format;
        attachment.samples = physical.get_msaa_samples();

        // Clear data at the start of the render pass
        attachment.loadOp = vk::AttachmentLoadOp::eClear;
        attachment.storeOp = vk::AttachmentStoreOp::eDontCare;

        // Ignore the stencil component
        attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

        // The layout at the end should be optimal
        attachment.initialLayout = vk::ImageLayout::eUndefined;
        attachment.finalLayout = vk::ImageLayout::eColorAttachmentOptimal;

        return attachment;
    }

    vk::AttachmentDescription RenderPass::create_depth_attachment() {
        const PhysicalDevice &physical = _device.get().get_physical();
        vk::AttachmentDescription attachment;
        attachment.format = physical.get_depth_format();
        attachment.samples = physical.get_msaa_samples();

        // Clear data at the start of the render pass
        attachment.loadOp = vk::AttachmentLoadOp::eClear;
        attachment.storeOp = vk::AttachmentStoreOp::eDontCare;

        // Ignore stencil component
        attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

        // The layout at the end should be optimal
        attachment.initialLayout = vk::ImageLayout::eUndefined;
        attachment.finalLayout =
            vk::ImageLayout::eDepthStencilAttachmentOptimal;

        return attachment;
    }

    vk::AttachmentDescription RenderPass::create_resolve_attachment() {
        vk::AttachmentDescription attachment;
        attachment.format = _swapchain.get().get_format().format;
        attachment.samples = vk::SampleCountFlagBits::e1;

        // Store the result in memory at the end of the render pass
        attachment.loadOp = vk::AttachmentLoadOp::eDontCare;
        attachment.storeOp = vk::AttachmentStoreOp::eStore;

        // Ignore stencil component
        attachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        attachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;

        // The layout at the end should be presentable to the screen
        attachment.initialLayout = vk::ImageLayout::eUndefined;
        attachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        return attachment;
    }

    std::vector<vk::SubpassDescription>
    RenderPass::create_subpasses(vk::AttachmentReference &color_ref,
                                 vk::AttachmentReference &depth_ref,
                                 vk::AttachmentReference &resolve_ref) {
        vk::SubpassDescription initial_subpass;
        initial_subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        initial_subpass.colorAttachmentCount = 1;
        initial_subpass.pColorAttachments = &color_ref;
        initial_subpass.pDepthStencilAttachment = &depth_ref;
        initial_subpass.pResolveAttachments = &resolve_ref;
        return {initial_subpass};
    }

    std::vector<vk::SubpassDependency>
    RenderPass::create_subpass_dependencies() {
        vk::SubpassDependency subpass_dependency;

        // Current subpass -> Next subpass
        subpass_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpass_dependency.dstSubpass = 0;

        // Pipeline stage dependences
        subpass_dependency.srcStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput |
            vk::PipelineStageFlagBits::eEarlyFragmentTests;
        subpass_dependency.dstStageMask =
            vk::PipelineStageFlagBits::eColorAttachmentOutput |
            vk::PipelineStageFlagBits::eEarlyFragmentTests;

        // Access flags
        subpass_dependency.srcAccessMask = vk::AccessFlagBits::eNoneKHR;
        subpass_dependency.dstAccessMask =
            vk::AccessFlagBits::eColorAttachmentWrite |
            vk::AccessFlagBits::eDepthStencilAttachmentWrite;

        // Only one dependency because we only have one subpass
        return {subpass_dependency};
    }

    const vk::RenderPass &RenderPass::get_handle() const { return _handle; }
} // namespace Dynamo::Graphics::Vulkan