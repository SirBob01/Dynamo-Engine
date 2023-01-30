#pragma once

#include <list>
#include <optional>
#include <unordered_map>

#include "../Log/Log.hpp"
#include "../Types.hpp"

namespace Dynamo {
    /**
     * @brief Round up a size to be a multiple of alignment.
     *
     * @param size      Size in bytes.
     * @param alignment Alignment in bytes.
     * @return u32
     */
    inline u32 align_size(u32 size, u32 alignment) {
        return ((size + alignment - 1) / alignment) * alignment;
    }

    /**
     * @brief Implements the allocation and deallocation strategy for
     * dynamic virtual heap memory management.
     *
     */
    class Allocator {
        struct Block {
            u32 offset;
            u32 size;
        };
        std::list<Block> _free;
        std::unordered_map<u32, u32> _used;

        u32 _capacity;

        /**
         * @brief Join adjacent blocks.
         *
         * @param it Reference block.
         */
        void defragment(std::list<Block>::iterator it);

      public:
        /**
         * @brief Construct a new Allocator object.
         *
         * @param capacity Capacity of the heap.
         */
        Allocator(u32 capacity);

        /**
         * @brief Reserve a block of memory with specific alignment
         * requirements, returning the offset within the pool.
         *
         * @param size      Desired size in bytes.
         * @param alignment Alignment requirement in bytes.
         * @return std::optional<u32>
         */
        std::optional<u32> reserve(u32 size, u32 alignment);

        /**
         * @brief Free the block of reserved memory at an offset.
         *
         * @param offset Offset within the pool in bytes returned by reserve().
         */
        void free(u32 offset);

        /**
         * @brief Grow the total capacity, expanding the free blocks.
         *
         * @param capacity New capacity in bytes >= current_capacity.
         */
        void grow(u32 capacity);

        /**
         * @brief Check if an offset is mapped to a reserved block.
         *
         * @param offset Offset within the pool in bytes returned by reserve().
         * @return true
         * @return false
         */
        b8 is_reserved(u32 offset) const;

        /**
         * @brief Get the capacity of the allocator.
         *
         * @return u32
         */
        u32 capacity() const;

        /**
         * @brief Get the size of a reserved block.
         *
         * @param offset Offset within the pool in bytes returned by reserve().
         * @return u32
         */
        u32 size(u32 offset) const;

        /**
         * @brief Generate the human-readable string to visualize the state
         * of the allocator (for debugging).
         *
         * @return std::string
         */
        std::string print() const;
    };
} // namespace Dynamo