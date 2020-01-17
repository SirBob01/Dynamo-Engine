#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include <vector>
#include <queue>
#include <cinttypes>

namespace Dynamo {
    // 2^32 - 1 possible unique entities at any instance
    typedef uint64_t Entity;

    class EntityTracker {
        uint32_t index_counter_;

        std::vector<uint32_t> versions_;
        std::queue<uint32_t> free_pool_;

    public:
        EntityTracker();

        // Check if an entity is still active
        bool is_active(Entity entity);

        // Create a new entity handle
        uint64_t create_entity();

        // Destroy an entity and recycle the index
        void destroy_entity(Entity entity);

        // Reset the index and version pool
        void clear();
    };
}

#endif