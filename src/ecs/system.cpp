#include "system.h"

namespace Dynamo {
    System::System(ComponentRegistry *component_registry) {
        registry_ = component_registry;
    }

    void System::register_entity(Entity entity) {
        auto it = std::find(
            entities_.begin(), 
            entities_.end(), 
            entity
        );
        if(it != entities_.end()) {
            return;
        }
        entities_.push_back(entity);
    }

    void System::unregister_entity(Entity entity) {
        entities_.erase(
            std::remove(entities_.begin(), entities_.end(), entity),
            entities_.end()
        );
    }

    void System::update(unsigned dt) {
        for(auto &entity : entities_) {
            tick(entity, dt);
        }
    }
}