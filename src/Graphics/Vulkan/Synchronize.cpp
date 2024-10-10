#include <Graphics/Vulkan/Synchronize.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    VkFence VkFence_build(VkDevice device) {
        VkFenceCreateInfo fence_info = {};
        fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VkFence fence;
        VkResult_log("Create Fence", vkCreateFence(device, &fence_info, nullptr, &fence));
        return fence;
    }

    VkSemaphore VkSemaphore_build(VkDevice device) {
        VkSemaphoreCreateInfo semaphore_info = {};
        semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkSemaphore semaphore;
        VkResult_log("Create Semaphore", vkCreateSemaphore(device, &semaphore_info, nullptr, &semaphore));
        return semaphore;
    }
} // namespace Dynamo::Graphics::Vulkan