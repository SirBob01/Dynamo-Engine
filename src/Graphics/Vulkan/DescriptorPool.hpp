#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "../../Types.hpp"
#include "./Device.hpp"
#include "./PipelineLayout.hpp"
#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Descriptor sets grouped by layout and ordered by set index
     *
     */
    using DescriptorSetGroup = std::vector<vk::DescriptorSet>;

    /**
     * @brief Maximum number of descriptor sets per pool
     *
     */
    constexpr u32 MAX_DESCRIPTOR_SETS_PER_POOL = 1000;

    /**
     * @brief Wrapper class abstraction over Vulkan descriptor pool
     *
     */
    class DescriptorPool {
        std::vector<vk::DescriptorPool> _pools;
        std::reference_wrapper<Device> _device;

        /**
         * @brief Create a new descriptor pool
         *
         * @param layout   Descriptor set layout
         * @param bindings List of bindings for the layout
         * @return vk::DescriptorPool
         */
        vk::DescriptorPool create_pool(const vk::DescriptorSetLayout &layout,
                                       const LayoutBindings &bindings);

        /**
         * @brief Allocate descriptor sets for a given layout
         *
         * @param layout   Descriptor set layout
         * @param bindings List of bindings for the layout
         * @param pool     Descriptor pool
         * @param count    Number of descriptor sets to create
         * @return std::vector<vk::DescriptorSet>
         */
        std::vector<vk::DescriptorSet>
        allocate_set(const vk::DescriptorSetLayout &layout,
                     const LayoutBindings &bindings,
                     const vk::DescriptorPool &pool,
                     u32 count);

        /**
         * @brief Select a pool and allocate
         *
         * @param layout   Descriptor set layout
         * @param bindings List of bindings
         * @param count    Number of descriptor sets to create
         * @return std::vector<vk::DescriptorSet>
         */
        std::vector<vk::DescriptorSet>
        try_allocate(const vk::DescriptorSetLayout &layout,
                     const LayoutBindings &bindings,
                     u32 count);

      public:
        /**
         * @brief Construct a new DescriptorPool object
         *
         * @param device    Reference to the logical device
         */
        DescriptorPool(Device &device);

        /**
         * @brief Destroy the DescriptorPool object
         *
         */
        ~DescriptorPool();

        /**
         * @brief Allocate descriptor set groups
         *
         * @param pipeline_layout Reference to the pipeline layout
         * @param swapchain       Reference to the swapchain
         * @return std::vector<DescriptorSetGroup>
         */
        std::vector<DescriptorSetGroup>
        allocate(PipelineLayout &pipeline_layout, Swapchain &swapchain);

        /**
         * @brief Reset all descriptor pools
         *
         */
        void reset();
    };
} // namespace Dynamo::Graphics::Vulkan