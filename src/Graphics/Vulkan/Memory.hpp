#pragma once

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Default capacity of the memory allocated for a pool
     *
     */
    constexpr unsigned MEMORY_POOL_CAPACITY = 256 * 1024 * 1024;

    /**
     * @brief Round up a number to the next highest multiple
     *
     * @param x
     * @param multiple
     * @return unsigned
     */
    inline unsigned round_up(unsigned x, unsigned multiple) {
        unsigned rem = x % multiple;
        return rem == 0 ? x : x + (multiple - rem);
    }

    /**
     * @brief Properties of a memory pool
     *
     * All blocks within a pool must have these properties
     *
     */
    struct MemoryPoolProperties {
        /**
         * @brief Memory type
         *
         */
        int type;

        /**
         * @brief Byte alignment
         *
         */
        unsigned alignment;

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        inline bool operator==(const MemoryPoolProperties &rhs) const {
            return type == rhs.type && alignment == rhs.alignment;
        }
    };

    /**
     * @brief Unique handle to allocated memory
     *
     */
    struct MemoryHandle {
        /**
         * @brief Properties of the memory pool
         *
         */
        MemoryPoolProperties properties;

        /**
         * @brief Memory pool index
         *
         */
        int pool_index;

        /**
         * @brief Memory block index
         *
         */
        int block_index;
    };

    /**
     * @brief Block of memory
     *
     */
    struct MemoryBlock {
        /**
         * @brief Size of the block
         *
         */
        unsigned size;

        /**
         * @brief Capacity of the block
         *
         */
        unsigned capacity;

        /**
         * @brief Offset within the buffer
         *
         */
        unsigned offset;
    };

} // namespace Dynamo::Graphics::Vulkan

/**
 * @brief Custom hash for ImageMemoryRequirements
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Graphics::Vulkan::MemoryPoolProperties> {
    inline size_t
    operator()(const Dynamo::Graphics::Vulkan::MemoryPoolProperties &properties)
        const {
        return ((properties.type ^ (properties.alignment << 1)) >> 1);
    }
};