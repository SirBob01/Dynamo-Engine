#include "world.h"

namespace Dynamo {
    World::~World() {
        for(auto &system : systems_) {
            delete system;
        }
    }

    Entity World::create_entity() {
        return entities_.generate_id();
    }

    void World::kill_entity(Entity entity) {
        components_.destroy_entity(entity);
        entities_.invalidate_id(entity);
    }

    template <typename Component>
    void World::add_component(Entity entity, Component prefab) {
        if(!entities_.is_active(entity)) {
            return;
        }
        components_.set_component<Component>(entity, prefab);
    }

    template <class S>
    void World::register_system() {
        std::type_index type = typeid(S);
        auto it = std::find(
            system_types_.begin(), 
            system_types_.end(), 
            type
        );
        if(it != system_types_.end()) {
            return;
        }
        systems_.push_back(new S(&components_));
        system_types_.push_back(type);
    }

    void World::update(unsigned dt) {
        for(auto &system : systems_) {
            system->update(dt);
        }
    }
}