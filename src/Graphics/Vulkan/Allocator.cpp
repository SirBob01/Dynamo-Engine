#include "./Allocator.hpp"

namespace Dynamo::Graphics::Vulkan {
    Allocator::Allocator(unsigned capacity) {
        // Create the initial free block that encompasses the heap
        Block heap_block;
        heap_block.offset = 0;
        heap_block.size = capacity;
        _free.push_back(heap_block);
    }

    void Allocator::defragment(std::list<Block>::iterator it) {
#ifdef DYN_DEBUG
        Log::info("Defragmentation target: {} {}",
                  it->offset,
                  it->offset + it->size);
        Log::info("Before: {}", print());
#endif
        Block block = *it;
        unsigned new_offset = block.offset;
        unsigned new_size = block.size;

        // Scan left
        if (it != _free.begin()) {
            auto l = std::prev(it);
            unsigned left_bound = block.offset;
            while (l->offset + l->size == left_bound) {
                new_offset -= l->size;
                new_size += l->size;
                left_bound = new_offset;

                // Advance backward
                if (l != _free.begin()) {
                    auto prev = std::prev(l);
                    _free.erase(l);
                    l = prev;
                } else {
                    _free.erase(l);
                }
            }
        }

        // Scan right
        if (it != _free.end()) {
            auto r = std::next(it);
            unsigned right_bound = block.offset + block.size;
            while (r != _free.end() && r->offset == right_bound) {
                new_size += r->size;
                right_bound += r->size;
                r = _free.erase(r);
            }
        }

        it->offset = new_offset;
        it->size = new_size;

#ifdef DYN_DEBUG
        Log::info("After: {}", print());
        Log::info("");
#endif
    }

    std::optional<unsigned> Allocator::reserve(unsigned size,
                                               unsigned alignment) {
        for (auto it = _free.begin(); it != _free.end(); it++) {
            Block &block = *it;

            unsigned offset = align_size(block.offset, alignment);
            unsigned block_r = block.offset + block.size;
            unsigned alloc_r = offset + size;

            if (block_r >= alloc_r) {
                // Handle allocation in the middle of the block due to alignment
                if (offset > block.offset) {
                    Block partition;
                    partition.offset = block.offset;
                    partition.size = offset - block.offset;
                    _free.insert(it, partition);
                }

                // Update the size and offset of the current block
                block.size = block_r - alloc_r;
                block.offset = alloc_r;

                // Block is empty, remove from freelist
                if (block.size == 0) {
                    _free.erase(it);
                }

                // Sanity check
                DYN_ASSERT(_used.count(offset) == 0);

                // Return the offset to the allocation and track it
                _used[offset] = size;
                return offset;
            }
        }
        return {};
    }

    void Allocator::free(unsigned offset) {
        if (_used.count(offset) == 0) {
            Log::error("Allocator::free() failed, invalid offset.");
        }

        // New free block
        Block freed;
        freed.offset = offset;
        freed.size = _used[offset];
        _used.erase(offset);

        // No free blocks
        if (_free.empty()) {
            _free.push_front(freed);
            return;
        }

        // Start of the list
        unsigned alloc_r = freed.offset + freed.size;
        if (_free.begin()->offset >= alloc_r) {
            _free.push_front(freed);
            defragment(_free.begin());
            return;
        }

        for (auto it = _free.begin(); it != _free.end(); it++) {
            unsigned block_r = it->offset + it->size;

            // End of the list
            auto next = std::next(it);
            if (next == _free.end()) {
                DYN_ASSERT(offset >= block_r);
                _free.push_back(freed);
                defragment(it);
                return;
            }

            // Between two blocks
            if (next->offset >= alloc_r && offset >= block_r) {
                defragment(_free.insert(next, freed));
                return;
            }
        }

        // Uh oh.
        DYN_ASSERT(false);
    }

    std::string Allocator::print() {
        std::string str;
        if (_free.empty()) {
            return str;
        }

        // Allocation before the first free block
        auto it = _free.begin();
        if (it->offset > 0) {
            str += " | ********";
        }

        while (it != _free.end()) {
            unsigned right = it->offset + it->size;
            str += " | " + std::to_string(it->offset) + ", " +
                   std::to_string(right);

            // Show the allocation before the next free block
            auto next = std::next(it);
            if (next != _free.end() && next->offset != right) {
                str += " | ********";
            }
            it++;
        }
        str += " | ";
        return str;
    }
} // namespace Dynamo::Graphics::Vulkan