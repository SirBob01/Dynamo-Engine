#ifndef DYNAMO_WORLD_H_
#define DYNAMO_WORLD_H_

#include <vector>
#include <unordered_map>

#include "entity.h"
#include "component.h"

namespace Dynamo {
    class World {
        EntityTracker entities_;

        std::unordered_map<Entity, std::vector<unsigned>> owned_;
        std::vector<BasePool *> pools_;

    public:
        ~World();

        // Create an new entity
        Entity create_entity();

        // Destroy an entity
        void destroy_entity(Entity entity);

        template <typename Component>
        BasePool *get_pool() {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= pools_.size()) {
                return nullptr;
            }
            return pools_[type_index];
        } 

        // Get an entity's component
        template <typename Component>
        Component *get_component(Entity entity) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= pools_.size()) {
                return nullptr;
            }
            if(!entities_.is_active(entity)) {
                return nullptr;
            }

            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                pools_[type_index]
            );
            int component_index = pool->search(entity);

            if(component_index == -1) {
                return nullptr;
            }
            return pool->get_at(component_index);
        }

        // Add a component to an entity
        template <typename Component, typename ... Fields>
        void add_component(Entity entity, Fields ... params) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= pools_.size()) {
                pools_.push_back(new ComponentPool<Component>());
            }
            if(!entities_.is_active(entity)) {
                return;
            }

            ComponentPool<Component> *pool;
            pool = dynamic_cast<ComponentPool<Component> *>(
                pools_[type_index]
            );
            pool->insert(entity, params ...);
            owned_[entity].push_back(type_index);
        }

        // Remove a component from an entity
        template <typename Component>
        void remove_component(Entity entity) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= pools_.size()) {
                return;
            }
            if(!entities_.is_active(entity)) {
                return;
            }

            ComponentPool<Component> *pool;
            pool = dynamic_cast<ComponentPool<Component> *>(
                pools_[type_index]
            );
            pool->remove(entity);
            owned_[entity].erase(
                std::remove(
                    owned_[entity].begin(),
                    owned_[entity].end(),
                    type_index
                ),
                owned_[entity].end()
            );
        }

        template <typename Component>
        void clear() {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= pools_.size()) {
                return;
            }
            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                pools_[type_index]
            );
            pool->clear();
        }
    };
}

#endif