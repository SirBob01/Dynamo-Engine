#pragma once

#include <vulkan/vulkan.h>

#include <Display.hpp>

namespace Dynamo::Graphics::Vulkan {
    constexpr std::array<const char *, 1> VALIDATION_LAYERS = {
        "VK_LAYER_KHRONOS_validation",
    };

    class Instance {
        VkInstance _handle;

        /**
         * @brief Check if all required validation layers are supported.
         *
         */
        void check_validation_layers() const;

      public:
        Instance(const Display &display);
        ~Instance();

        const VkInstance &handle();
    };
} // namespace Dynamo::Graphics::Vulkan