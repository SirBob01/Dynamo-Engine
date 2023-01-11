#pragma once

#include <list>
#include <optional>
#include <unordered_map>

#include "../../Log/Log.hpp"
#include "../../Utils/Bits.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Round up a size to be a multiple of alignment
     *
     * @param size      Size in bytes
     * @param alignment Alignment in bytes
     * @return unsigned
     */
    inline unsigned align_size(unsigned size, unsigned alignment) {
        return ((size + alignment - 1) / alignment) * alignment;
    }

    /**
     * @brief Implements the block allocation and deallocation strategy for
     * dynamic heap memory management
     *
     */
    class Allocator {
        struct Block {
            unsigned offset;
            unsigned size;
        };
        std::list<Block> _free;
        std::unordered_map<unsigned, unsigned> _used;

        /**
         * @brief Join adjacent blocks
         *
         * @param it Reference block
         */
        void defragment(std::list<Block>::iterator it);

      public:
        /**
         * @brief Construct a new Allocator object
         *
         * @param capacity Capacity of the heap
         */
        Allocator(unsigned capacity);

        /**
         * @brief Reserve a block of memory with specific alignment
         * requirements, returning the offset within the pool.
         *
         * This find the first free block that will fit the size (with the given
         * offset alignment) and allocate.
         *
         * @param size      Desired size in bytes
         * @param alignment Alignment requirement in bytes
         * @return std::optional<unsigned>
         */
        std::optional<unsigned> reserve(unsigned size, unsigned alignment);

        /**
         * @brief Free the block of reserved memory at an offset
         *
         * This will iterate over the free blocks and find the first one whose
         * right boundary coincides with the allocated offset. This block and
         * the next are then merged together, minimizing defragmentation.
         *
         * @param offset Offset within the pool in bytes returned by reserve()
         */
        void free(unsigned offset);

        /**
         * @brief Generate the human-readable string to visualize the state
         * of the allocator (for debugging)
         *
         */
        std::string print();
    };
} // namespace Dynamo::Graphics::Vulkan