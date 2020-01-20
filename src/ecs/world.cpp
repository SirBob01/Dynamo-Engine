#include "world.h"

namespace Dynamo {
    Entity World::create_entity() {
        return entities_.generate_id();
    }

    void World::destroy_entity(Entity entity) {
        // Destroy all component instances
        if(!entities_.is_active(entity)) {
            return;
        }
        for(auto &pool : components_) {
            pool->remove(EntityTracker::get_index(entity));
        }
        entities_.invalidate_id(entity);
    }
}