#include "./DescriptorAllocator.hpp"

namespace Dynamo::Graphics::Vulkan {
    DescriptorAllocator::DescriptorAllocator(Device &device) :
        _device(device) {}

    DescriptorAllocator::~DescriptorAllocator() {
        for (const vk::DescriptorPool &pool : _pools) {
            _device.get().get_handle().destroyDescriptorPool(pool);
        }
    }

    vk::DescriptorPool
    DescriptorAllocator::create_pool(const vk::DescriptorSetLayout &layout,
                                     const LayoutBindings &bindings) {
        // Determine the pool sizes from the bindings associated with the layout
        std::vector<vk::DescriptorPoolSize> pool_sizes;
        for (const vk::DescriptorSetLayoutBinding &binding : bindings) {
            vk::DescriptorPoolSize pool_size;
            pool_size.type = binding.descriptorType;
            pool_size.descriptorCount =
                MAX_DESCRIPTOR_SETS_PER_POOL * binding.descriptorCount;
            pool_sizes.push_back(pool_size);
        }

        vk::DescriptorPoolCreateInfo pool_info;
        pool_info.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;
        pool_info.maxSets = MAX_DESCRIPTOR_SETS_PER_POOL;
        pool_info.poolSizeCount = pool_sizes.size();
        pool_info.pPoolSizes = pool_sizes.data();

        return _device.get().get_handle().createDescriptorPool(pool_info);
    }

    std::vector<vk::DescriptorSet>
    DescriptorAllocator::allocate_set(const vk::DescriptorSetLayout &layout,
                                      const LayoutBindings &bindings,
                                      const vk::DescriptorPool &pool,
                                      Swapchain &swapchain) {
        unsigned image_count = swapchain.get_images().size();

        // Allocate new descriptor sets within the pool
        std::vector<vk::DescriptorSetLayout> set_layouts(image_count, layout);
        vk::DescriptorSetAllocateInfo alloc_info;
        alloc_info.descriptorPool = pool;
        alloc_info.descriptorSetCount = set_layouts.size();
        alloc_info.pSetLayouts = set_layouts.data();

        return _device.get().get_handle().allocateDescriptorSets(alloc_info);
    }

    std::vector<vk::DescriptorSet>
    DescriptorAllocator::try_allocate(const vk::DescriptorSetLayout &layout,
                                      const LayoutBindings &bindings,
                                      Swapchain &swapchain) {
        bool found_pool = false;
        std::vector<vk::DescriptorSet> vksets;
        for (vk::DescriptorPool &pool : _pools) {
            try {
                vksets = allocate_set(layout, bindings, pool, swapchain);
                found_pool = true;
                break;
            } catch (vk::OutOfPoolMemoryError &err) {
                continue;
            } catch (vk::FragmentedPoolError &err) {
                continue;
            }
        }

        // Create a new pool if no compatible one was found
        if (!found_pool) {
            vk::DescriptorPool pool = create_pool(layout, bindings);
            _pools.push_back(pool);
            vksets = allocate_set(layout, bindings, pool, swapchain);
        }
        return vksets;
    }

    std::vector<DescriptorSetGroup>
    DescriptorAllocator::allocate(PipelineLayout &pipeline_layout,
                                  Swapchain &swapchain) {
        const std::vector<vk::DescriptorSetLayout> &layouts =
            pipeline_layout.get_descriptor_set_layouts();
        const std::vector<LayoutBindings> &bindings =
            pipeline_layout.get_descriptor_set_bindings();

        std::vector<DescriptorSetGroup> groups(layouts.size());
        for (unsigned i = 0; i < layouts.size(); i++) {
            for (vk::DescriptorSet &vkset :
                 try_allocate(layouts[i], bindings[i], swapchain)) {
                groups[i].push_back(
                    std::make_unique<DescriptorSet>(_device.get(), vkset));
            }
        }
        return groups;
    }

    void DescriptorAllocator::reset() {
        // Reset all active descriptor pools and move them to the inactive queue
        for (vk::DescriptorPool &pool : _pools) {
            _device.get().get_handle().resetDescriptorPool(pool);
        }
    }
} // namespace Dynamo::Graphics::Vulkan