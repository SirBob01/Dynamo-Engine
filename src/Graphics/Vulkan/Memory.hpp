#pragma once

#include <functional>
#include <list>
#include <optional>

#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "../../Utils/Allocator.hpp"
#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for Vulkan device memory
     *
     */
    class Memory {
        vk::DeviceMemory _handle;
        std::reference_wrapper<Device> _device;
        Allocator _allocator;

        vk::MemoryType _type;
        unsigned _capacity;

        char *_mapped;

      public:
        /**
         * @brief Construct a new Memory object
         *
         * @param device       Reference to the logical device
         * @param requirements Memory allocation requirements
         * @param properties   Memory properties
         */
        Memory(Device &device,
               vk::MemoryRequirements requirements,
               vk::MemoryPropertyFlags properties);

        /**
         * @brief Destroy the Memory object
         *
         */
        ~Memory();

        /**
         * @brief Get the handle to vk::DeviceMemory
         *
         * @return const vk::DeviceMemory&
         */
        const vk::DeviceMemory &get_handle() const;

        /**
         * @brief Get the memory type
         *
         * @return const vk::MemoryType&
         */
        const vk::MemoryType &get_type() const;

        /**
         * @brief Get the device
         *
         * @return Device&
         */
        Device &get_device();

        /**
         * @brief Get the total capacity of the memory
         *
         * @return unsigned
         */
        unsigned get_capacity() const;

        /**
         * @brief Read from mapped memory
         *
         * @param dst    Destination buffer
         * @param offset Offset within the memory map in bytes
         * @param length Length of the read in bytes
         */
        void read(char *dst, unsigned offset, unsigned length);

        /**
         * @brief Write to mapped memory
         *
         * @param src    Source buffer
         * @param offset Offset within the memory map in bytes
         * @param length Length of the write in bytes
         */
        void write(char *src, unsigned offset, unsigned length);

        /**
         * @brief Bind a vk::Image to the underlying vk::Memory
         *
         * @param vkimage Handle to vk::Image
         * @param offset  Offset within the memory map in bytes
         */
        void bind(vk::Image vkimage, unsigned offset);

        /**
         * @brief Bind a vk::Buffer to the underlying vk::Memory
         *
         * @param vkbuffer Handle to vk::Buffer
         * @param offset   Offset within the memory map in bytes
         */
        void bind(vk::Buffer vkbuffer, unsigned offset);

        /**
         * @brief Reserve a block of memory with specific alignment
         * requirements, returning the offset within the pool
         *
         * @param size      Desired size in bytes
         * @param alignment Alignment requirement in bytes
         * @return std::optional<unsigned>
         */
        std::optional<unsigned> reserve(unsigned size, unsigned alignment);

        /**
         * @brief Free the block of reserved memory at an offset
         *
         * @param offset Offset within the pool in bytes returned by reserve()
         */
        void free(unsigned offset);
    };
} // namespace Dynamo::Graphics::Vulkan
