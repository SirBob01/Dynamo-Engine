#pragma once

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"
#include "./Image.hpp"
#include "./Memory.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Pool of image memory blocks
     *
     */
    class ImagePool {
        Device &_device;
        unsigned _capacity;
        unsigned _alignment;

        vk::UniqueDeviceMemory _memory;
        std::vector<MemoryBlock> _blocks;
        std::unordered_set<unsigned> _recycle;

        /**
         * @brief Recycle an existing block, returning the block index
         *
         * Returns -1 on failure
         *
         * @param image
         * @return int
         */
        int recycle_memory(Image &image);

        /**
         * @brief Allocate a new block, returning the block index
         *
         * Returns -1 on failure
         *
         * @param image
         * @return int
         */
        int allocate_memory(Image &image);

      public:
        /**
         * @brief Construct a new ImagePool object
         *
         * @param device
         * @param properties
         */
        ImagePool(Device &device, MemoryPoolProperties &properties);

        /**
         * @brief Add an image to the pool and return an index to the block
         *
         * This will attempt to defragment memory by recycling blocks
         *
         * @param image
         * @return int
         */
        int add(Image &image);

        /**
         * @brief Deallocate the block at an index
         *
         * @param block_index
         */
        void remove(int block_index);
    };

    /**
     * @brief Manages the memory for images (e.g., textures, swapchain images)
     *
     */
    class ImageAllocator {
        Device &_device;

        std::unordered_map<MemoryPoolProperties,
                           std::vector<std::unique_ptr<ImagePool>>>
            _pools;

        /**
         * @brief Get the required pool properties of an image
         *
         * @param image
         * @return MemoryPoolProperties
         */
        MemoryPoolProperties get_pool_properties(Image &image);

      public:
        /**
         * @brief Construct a new ImageAllocator object
         *
         * @param device
         */
        ImageAllocator(Device &device);

        /**
         * @brief Allocate memory for an image and return a handle to the
         * created block
         *
         * If an image cannot be added to an existing pool, a new one is created
         *
         * @param image
         * @return ImageHandle
         */
        MemoryHandle allocate(Image &image);

        /**
         * @brief Deallocate a memory block
         *
         * @param handle
         */
        void deallocate(MemoryHandle &handle);

        /**
         * @brief Reset all pools, assuming that all images bound to the pools
         * have been destroyed
         *
         */
        void reset();
    };
} // namespace Dynamo::Graphics::Vulkan