#pragma once

#include <vulkan/vulkan.h>

#include <Graphics/Vulkan/Instance.hpp>

namespace Dynamo::Graphics::Vulkan {
    class Surface {
        Instance &_instance;
        VkSurfaceKHR _handle;

      public:
        Surface(Instance &instance, const Display &display);
        ~Surface();

        VkSurfaceKHR handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan