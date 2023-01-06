#pragma once

#include <functional>
#include <optional>
#include <vector>

#include "../../Utils/Bits.hpp"
#include "./Device.hpp"
#include "./Memory.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Minimum capacity of a single memory allocation
     *
     */
    constexpr unsigned MINIMUM_MEMORY_CAPACITY = 256 * (1 << 20);

    /**
     * @brief Allocation result
     *
     */
    struct Allocation {
        /**
         * @brief Reference to the memory
         *
         */
        const std::reference_wrapper<Memory> memory;

        /**
         * @brief Reserved offset within the memory
         *
         */
        const unsigned offset;
    };

    /**
     * @brief Dynamic memory allocator
     *
     * Memory objects are grouped based on their type so allocations with
     * desired properties can be queried
     *
     */
    class Allocator {
        std::reference_wrapper<Device> _device;

        using Pool = std::vector<std::unique_ptr<Memory>>;
        Pool _pools;

        /**
         * @brief Check if a memory pool is compatible with the given
         * requirements and properties
         *
         * @param memory
         * @param requirements
         * @param properties
         * @return true
         * @return false
         */
        bool is_compatible(Memory &memory,
                           vk::MemoryRequirements requirements,
                           vk::MemoryPropertyFlagBits properties);

      public:
        /**
         * @brief Construct a new Allocator object
         *
         * @param device Reference to the logical device
         */
        Allocator(Device &device);

        /**
         * @brief Allocate a block of memory
         *
         * @param requirements
         * @param properties
         * @return std::optional<Allocation>
         */
        std::optional<Allocation>
        allocate(vk::MemoryRequirements requirements,
                 vk::MemoryPropertyFlagBits properties);
    };
} // namespace Dynamo::Graphics::Vulkan