#include "./ImageAllocator.hpp"

namespace Dynamo::Graphics::Vulkan {
    ImagePool::ImagePool(Device &device, MemoryPoolProperties &properties) :
        _device(device) {
        _capacity = MEMORY_POOL_CAPACITY;
        _alignment = properties.alignment;

        vk::MemoryAllocateInfo alloc_info;
        alloc_info.allocationSize = MEMORY_POOL_CAPACITY;
        alloc_info.memoryTypeIndex = properties.type;

        _memory = _device.get_handle().allocateMemoryUnique(alloc_info);
    }

    int ImagePool::recycle_memory(Image &image) {
        vk::MemoryRequirements requirements = image.get_memory_requirements();
        for (const unsigned &index : _recycle) {
            MemoryBlock &block = _blocks[index];
            if (block.capacity >= requirements.size) {
                _device.get_handle().bindImageMemory(image.get_handle(),
                                                     _memory.get(),
                                                     block.offset);
                _recycle.erase(index);
                return index;
            }
        }
        return -1;
    }

    int ImagePool::allocate_memory(Image &image) {
        vk::MemoryRequirements requirements = image.get_memory_requirements();
        MemoryBlock block;
        block.capacity = round_up(requirements.size, _alignment);
        if (_blocks.empty()) {
            block.offset = 0;
        } else {
            MemoryBlock &tail = _blocks.back();
            block.offset = tail.offset + tail.capacity;
        }

        // Required block is too large for this pool
        if (block.offset + block.capacity > _capacity) {
            return -1;
        } else {
            _device.get_handle().bindImageMemory(image.get_handle(),
                                                 _memory.get(),
                                                 block.offset);
            _blocks.push_back(block);
            return _blocks.size() - 1;
        }
    }

    int ImagePool::add(Image &image) {
        // TODO: Attempt defragmentation on failure
        int index = recycle_memory(image);
        if (index < 0) {
            index = allocate_memory(image);
        }
        return index;
    }

    void ImagePool::remove(int block_index) { _recycle.insert(block_index); }

    ImageAllocator::ImageAllocator(Device &device) : _device(device) {}

    MemoryPoolProperties ImageAllocator::get_pool_properties(Image &image) {
        vk::MemoryRequirements image_memory_properties =
            image.get_memory_requirements();
        vk::PhysicalDeviceMemoryProperties device_memory_properties =
            _device.get_physical().get_memory_properties();
        vk::MemoryPropertyFlagBits properties =
            vk::MemoryPropertyFlagBits::eDeviceLocal;

        MemoryPoolProperties pool_properties;
        for (unsigned i = 0; i < device_memory_properties.memoryTypeCount;
             i++) {
            if ((image_memory_properties.memoryTypeBits & (1 << i)) &&
                (device_memory_properties.memoryTypes[i].propertyFlags &
                 properties)) {

                pool_properties.type = i;
                pool_properties.alignment = image_memory_properties.alignment;
                return pool_properties;
            }
        }
        Log::error("Vulkan failed to allocate memory for an image.");
        return pool_properties;
    }

    MemoryHandle ImageAllocator::allocate(Image &image) {
        MemoryPoolProperties pool_properties = get_pool_properties(image);
        std::vector<std::unique_ptr<ImagePool>> &pools =
            _pools[pool_properties];

        // Find a compatible pool with available space and bind the image
        int block_index = -1;
        int pool_index;
        for (pool_index = 0; pool_index < pools.size(); pool_index++) {
            block_index = pools[pool_index]->add(image);
            if (block_index >= 0) {
                break;
            }
        }

        // If none of the pools are available, create a new one
        if (block_index < 0) {
            pools.push_back(
                std::make_unique<ImagePool>(_device, pool_properties));
            pool_index = pools.size() - 1;
            block_index = pools.back()->add(image);
        }

        MemoryHandle handle;
        handle.properties = pool_properties;
        handle.pool_index = pool_index;
        handle.block_index = block_index;
        return handle;
    }

    void ImageAllocator::deallocate(MemoryHandle &handle) {
        _pools[handle.properties][handle.pool_index]->remove(
            handle.block_index);
    }

    void ImageAllocator::reset() { _pools.clear(); }
} // namespace Dynamo::Graphics::Vulkan