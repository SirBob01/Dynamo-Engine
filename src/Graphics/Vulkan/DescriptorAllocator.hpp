#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "./DescriptorSet.hpp"
#include "./Device.hpp"
#include "./PipelineLayout.hpp"
#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Group of descriptor sets
     *
     */
    using DescriptorSetGroup = std::vector<std::unique_ptr<DescriptorSet>>;

    /**
     * @brief Maximum number of descriptor sets per pool
     *
     */
    constexpr unsigned MAX_DESCRIPTOR_SETS_PER_POOL = 1000;

    /**
     * @brief Wrapper class abstraction over Vulkan descriptor pool
     *
     */
    class DescriptorAllocator {
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
         * @return std::vector<vk::DescriptorSet>
         */
        std::vector<vk::DescriptorSet>
        allocate_set(const vk::DescriptorSetLayout &layout,
                     const LayoutBindings &bindings,
                     const vk::DescriptorPool &pool,
                     Swapchain &swapchain);

        /**
         * @brief Select a pool and allocate
         *
         * @param layout
         * @param bindings
         * @return std::vector<vk::DescriptorSet>
         */
        std::vector<vk::DescriptorSet>
        try_allocate(const vk::DescriptorSetLayout &layout,
                     const LayoutBindings &bindings,Swapchain &swapchain);

      public:
        /**
         * @brief Construct a new DescriptorAllocator object
         *
         * @param device    Reference to the logical device
         */
        DescriptorAllocator(Device &device);

        /**
         * @brief Destroy the DescriptorAllocator object
         *
         */
        ~DescriptorAllocator();

        /**
         * @brief Allocate descriptor sets grouped by layout and ordered by set
         * index
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