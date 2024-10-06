#pragma once

#include <vulkan/vulkan.h>

#include <Graphics/Vulkan/Device.hpp>

namespace Dynamo::Graphics::Vulkan {
    class Image {
      protected:
        Device &_device;
        VkImage _handle;
        VkFormat _format;
        VkMemoryRequirements _memory_requirements;

      public:
        Image(Device &device, VkImage handle, VkFormat format);

        VkImage handle() const;

        VkFormat format() const;

        VkMemoryRequirements memory_requirements() const;

        Device &device() const;
    };
} // namespace Dynamo::Graphics::Vulkan