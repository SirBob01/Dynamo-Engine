#include "ecs.h"

namespace Dynamo {
    EntityRegistry::EntityRegistry() {
        unique_id_ = 0;
    }

    Entity EntityRegistry::create_entity() {
        return unique_id_++;
    }

    void EntityRegistry::destroy_entity(Entity id) {
        // Destroy components
        auto &entity = entities_[id];
        for(auto &meta : entity) {
            auto &c_list = components_[meta.first];
            c_list.erase(
                std::remove(
                    c_list.begin(), 
                    c_list.end(), 
                    meta.second
                ), 
                c_list.end()
            );
            delete meta.second;
        }

        // Delete entity from database
        entities_.erase(id);
    }

    System::System(EntityRegistry *entities) {
        entities_ = entities;
    }
}