#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for the Vulkan descriptor set
     *
     */
    class DescriptorSet {
        vk::DescriptorSet _handle;
        std::reference_wrapper<Device> _device;

      public:
        /**
         * @brief Construct a new DescriptorSet object
         *
         * @param device
         * @param handle
         */
        DescriptorSet(Device &device, vk::DescriptorSet &handle);

        /**
         * @brief Destroy the DescriptorSet object
         *
         */
        ~DescriptorSet();

        /**
         * @brief Get the handle to vk::DescriptorSet
         *
         * @return const vk::DescriptorSet&
         */
        const vk::DescriptorSet &get_handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan