#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include <vector>
#include <queue>
#include <cinttypes>

namespace Dynamo {
    // 2^32 - 1 possible unique entities at any instance
    using Entity = uint64_t;

    // Keeps track of entity index versioning
    class EntityTracker {
        uint32_t index_counter_;

        std::vector<uint32_t> versions_;
        std::queue<uint32_t> free_pool_;

    public:
        EntityTracker();

        // Get the entity index
        static inline uint32_t get_index(Entity entity) {
            return entity >> 32;
        };

        // Get the entity version number
        static inline uint32_t get_version(Entity entity) {
            return entity & 0xFFFFFFFF;
        };

        // Check if an entity is still active
        bool is_active(Entity entity);

        // Create a new entity handle
        Entity generate_id();

        // Recycle an old index and invalidate the ID
        void invalidate_id(Entity entity);

        // Reset the index and version pool
        void clear();
    };
}

#endif