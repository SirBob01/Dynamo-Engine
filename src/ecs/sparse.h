#ifndef DYNAMO_SPARSE_H_
#define DYNAMO_SPARSE_H_

#include <cstdlib>
#include <cinttypes>

namespace Dynamo {
    // Dynamic sparse set for uint32_t
    class Sparse {
        int length_;
        int max_capacity_;
        int max_value_;

        uint32_t *dense_;
        uint32_t *sparse_;
    
    public:
        Sparse();
        ~Sparse();

        // Get the length of the sparse set
        int get_length();

        // Get an value from the dense array 
        // Use for iteration
        int get_at(int index);

        // Get the index of an item x if it exists
        int search(uint32_t x);

        // Insert a new value into the set
        void insert(uint32_t x);

        // Remove a value if it exists
        void remove(uint32_t x);
        
        // Clear the set
        void clear();
    };
}

#endif