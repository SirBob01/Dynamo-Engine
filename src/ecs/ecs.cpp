#include "ecs.h"

namespace Dynamo {
    ECS::~ECS() {
        for(auto &pool : pools_) {
            delete pool;
        }
    }

    Entity ECS::create_entity() {
        return entities_.generate_id();
    }

    void ECS::destroy_entity(Entity entity) {
        // Destroy all component instances
        if(!entities_.is_active(entity)) {
            return;
        }
        for(auto &type : owned_[entity]) {
            pools_[type.first]->remove(entity);
        }
        entities_.invalidate_id(entity);
        owned_.erase(entity);
    }
}