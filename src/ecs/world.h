#ifndef DYNAMO_WORLD_H_
#define DYNAMO_WORLD_H_

#include <vector>
#include <cstdlib>
#include <iostream>

#include "entity.h"
#include "component.h"

namespace Dynamo {
    class World {
        EntityTracker entities_;

        std::vector<BasePool *> components_;

    public:
        // Create an new entity
        Entity create_entity();

        // Destroy an entity
        void destroy_entity(Entity entity);

        // Get an entity's component
        template <typename Component>
        Component *get_component(Entity entity) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= components_.size()) {
                return nullptr;
            }
            if(!entities_.is_active(entity)) {
                return nullptr;
            }

            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                components_[type_index]
            );
            int component_index = pool->search(
                EntityTracker::get_index(entity)
            );

            if(component_index == -1) {
                return nullptr;
            }
            return pool->get_at(component_index);
        }

        // Add a component to an entity
        template <typename Component>
        void add_component(Entity entity, Component prefab) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= components_.size()) {
                components_.push_back(new ComponentPool<Component>());
            }
            if(!entities_.is_active(entity)) {
                return;
            }
            ComponentPool<Component> *pool;
            pool = dynamic_cast<ComponentPool<Component> *>(
                components_[type_index]
            );
            pool->insert(entity, prefab);
        }

        // Remove a component from an entity
        template <typename Component>
        void remove_component(Entity entity) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= components_.size()) {
                return;
            }
            if(!entities_.is_active(entity)) {
                return;
            }

            ComponentPool<Component> *pool;
            pool = dynamic_cast<ComponentPool<Component> *>(
                components_[type_index]
            );
            pool->remove(EntityTracker::get_index(entity));
        }
        
        template <typename Component>
        void clear() {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= components_.size()) {
                return;
            }
            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                components_[type_index]
            );
            pool->clear();
        }
    };
}

#endif