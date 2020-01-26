#ifndef DYNAMO_WORLD_H_
#define DYNAMO_WORLD_H_

#include <vector>
#include <unordered_map>

#include "entity.h"
#include "component.h"

namespace Dynamo {
    // ECS World manages all entities and component groups
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

        // Get a vector of entities belonging to a component group
        template <typename ... Component>
        std::vector<Entity> get_group() {
            std::vector<BasePool *> group_pools;
            std::vector<Entity> group;

            // Grab the pools of each component type
            // Uses initializer-list trick to unpack variadic-args
            std::initializer_list<int>{
                ((void)group_pools.push_back(
                    pools_[TypeID::get_id<Component>()]
                ), 0)...
            };

            // Get minimum length pool
            BasePool *min = group_pools[0];
            for(auto &pool : group_pools) {
                if(pool->get_length() < min->get_length()) {
                    min = pool;
                }
            }

            // Loop through entities in min and check if 
            // they exist in the other pools
            for(int i = 0; i < min->get_length(); i++) {
                Entity entity = min->get_entity(i);
                
                bool match = true;
                for(int j = 0; j < group_pools.size() && match; j++) {
                    if(group_pools[j] == min) {
                        continue;
                    }
                    if(group_pools[j]->search(entity) == -1) {
                        match = false;
                    }
                }
                if(match) {
                    group.push_back(entity);
                }
            }
            return group;
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

        // Perform a function on a particular pool of components
        template <typename Component, class F>
        void each(F function) {
            unsigned type_index = TypeID::get_id<Component>();
            if(type_index >= pools_.size()) {
                return;
            }
            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                pools_[type_index]
            );
            pool->each(function);
        }

        // Clear a component pool
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