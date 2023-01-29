#pragma once

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan pipeline cache
     *
     */
    class PipelineCache {
        vk::PipelineCache _handle;
        std::reference_wrapper<Device> _device;

      public:
        /**
         * @brief Construct a new PipelineCache object
         *
         * @param device Reference to the logical device
         */
        PipelineCache(Device &device);

        /**
         * @brief Destroy the PipelineCache object
         *
         */
        ~PipelineCache();

        /**
         * @brief Get the handle to vk::PipelineCache
         *
         * @return const vk::PipelineCache&
         */
        const vk::PipelineCache &get_handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan