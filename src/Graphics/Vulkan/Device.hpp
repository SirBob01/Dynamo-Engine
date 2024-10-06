#pragma once

#include <vulkan/vulkan.h>

#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/PhysicalDevice.hpp>

namespace Dynamo::Graphics::Vulkan {
    enum class QueueType {
        Graphics,
        Present,
        Transfer,
        Compute,
    };

    class Device {
        PhysicalDevice &_physical;
        VkDevice _handle;

        std::vector<VkDeviceQueueCreateInfo> get_queue_infos() const;

      public:
        Device(PhysicalDevice &physical);
        ~Device();

        VkDevice handle() const;

        const PhysicalDevice &physical() const;

        VkQueue queue(QueueType type, unsigned index) const;

        /**
         * @brief Wait for all queue operations to finish.
         *
         */
        void await() const;
    };
} // namespace Dynamo::Graphics::Vulkan