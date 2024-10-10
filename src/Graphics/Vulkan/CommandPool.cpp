#include <Graphics/Vulkan/CommandPool.hpp>
#include <Graphics/Vulkan/Utils.hpp>
#include <vulkan/vulkan_core.h>

namespace Dynamo::Graphics::Vulkan {
    VkCommandPool VkCommandPool_build(VkDevice device, QueueFamily family) {
        VkCommandPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = family.index;

        VkCommandPool pool;
        VkResult_log("Create Command Pool", vkCreateCommandPool(device, &pool_info, nullptr, &pool));
        return pool;
    }

    std::vector<VkCommandBuffer>
    VkCommandBuffer_allocate(VkDevice device, VkCommandPool pool, VkCommandBufferLevel level, unsigned count) {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.commandPool = pool;
        alloc_info.level = level;
        alloc_info.commandBufferCount = count;

        std::vector<VkCommandBuffer> buffers(count);
        VkResult_log("Allocate Command Buffers", vkAllocateCommandBuffers(device, &alloc_info, buffers.data()));
        return buffers;
    }
} // namespace Dynamo::Graphics::Vulkan