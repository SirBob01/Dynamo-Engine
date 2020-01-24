#include "world.h"

namespace Dynamo {
    World::~World() {
        for(auto &pool : pools_) {
            delete pool;
        }
    }

    Entity World::create_entity() {
        return entities_.generate_id();
    }

    void World::destroy_entity(Entity entity) {
        // Destroy all component instances
        if(!entities_.is_active(entity)) {
            return;
        }
        for(auto &type : owned_[entity]) {
            pools_[type]->remove(entity);
        }
        entities_.invalidate_id(entity);
        owned_.erase(entity);
    }
}