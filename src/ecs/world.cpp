#include "world.h"

namespace Dynamo {
    World::~World() {
        for(auto &system : systems_) {
            delete system;
        }
    }

    Entity World::create_entity() {
        return entities_.create_entity();
    }

    void World::kill_entity(Entity entity) {
        entities_.destroy_entity(Entity entity);
    }

    template <class S>
    void World::register_system() {
        std::type_index type = typeid(S);
        auto it = std::find(
            system_types_.begin(), 
            system_types_.end(), 
            type
        );
        if(it != system_types.end()) {
            return;
        }
        systems_.push_back(new S(&registry_));
        system_types.push_back(type);
    }

    void World::update(unsigned dt) {
        for(auto &system : systems_) {
            system->update(dt);
        }
    }
}