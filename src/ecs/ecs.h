#ifndef DYNAMO_ECS_H_
#define DYNAMO_ECS_H_

#include <vector>
#include <unordered_map>
#include <functional>

#include "entity.h"
#include "component.h"
#include "../util/typeid.h"

namespace Dynamo {
    // ECS class manages all entities and component groups
    class ECS {
        EntityTracker entities_;
        TypeID registry_;

        std::unordered_map<
            Entity, std::unordered_map<unsigned, int>
        > owned_;
        std::vector<BasePool *> pools_;

    public:
        ~ECS();

        // Function signature for working with components
        template <typename ... T>
        struct fn_sig {
            typedef std::function<void(T ...)> type;
        };

        // Create an new entity
        Entity create_entity();

        // Destroy an entity
        void destroy_entity(Entity entity);

        // Get a vector of entities belonging to a component group
        // Alternatively, pass a lambda that will be called on each entity
        template <typename ... Component>
        std::vector<Entity> group(typename fn_sig<Entity &, Component &...>
                                       ::type func=nullptr) {
            // Grab the pools of each component type
            // Uses initializer-list trick to unpack variadic-args
            std::vector<Entity> group;
            std::vector<int> pool_ids;
            int dummy[]{0,
                (pool_ids.push_back(
                    registry_.get_id<Component>()
                ), 0)...
            };

            // Find the index of minimum length pool
            int min_size = -1;
            int min_pos = -1;
            for(int i = 0; i < pool_ids.size(); i++) {
                int index = pool_ids[i];
                if(index >= pools_.size() || 
                   pools_[index] == nullptr) {
                    return group;
                }
                int curr_size = pools_[index]->get_length();
                if(min_size == -1 || curr_size < min_size) {
                    min_size = curr_size;
                    min_pos = i;
                }
            }
            std::swap(pool_ids[0], pool_ids[min_pos]);

            // Check if each entity in min pool exists in all other pools
            // Complexity: O(|group| * |min(group)|)
            // Can we do better?
            for(auto &entity : pools_[pool_ids[0]]->get_members()) {
                bool match = true;
                for(int j = 1; j < pool_ids.size() && match; j++) {
                    if(pools_[pool_ids[j]]->search(entity) == -1) {
                        match = false;
                    }
                }
                if(match) {
                    group.push_back(entity);
                    if(func) {
                        func(entity, *grab<Component>(entity) ...);
                    }
                }
            }
            return group;
        };

        // Get the pointer to an entity's component if it exists
        // TODO: Redesign ComponentPool so we won't need dynamic_cast
        template <typename Component>
        Component *grab(Entity entity) {
            unsigned type_index = registry_.get_id<Component>();
            if(type_index >= pools_.size() || 
               pools_[type_index] == nullptr || 
               !entities_.is_active(entity)) {
                return nullptr;
            }

            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                pools_[type_index]
            );
            return pool->get_at(pool->search(entity));
        };

        // Add a component to an entity
        template <typename Component, typename ... Fields>
        void assign(Entity entity, Fields ... params) {
            unsigned type_index = registry_.get_id<Component>();
            if(type_index >= pools_.size()) {
                pools_.resize(type_index + 1, nullptr);
            }
            if(pools_[type_index] == nullptr) {
                pools_[type_index] = new ComponentPool<Component>();   
            }
            if(!entities_.is_active(entity)) {
                return;
            }

            ComponentPool<Component> *pool;
            pool = dynamic_cast<ComponentPool<Component> *>(
                pools_[type_index]
            );
            if(pool->search(entity) == -1) {
                pool->insert(entity, params ...);
                owned_[entity][type_index] = 1;
            }
        };

        // Remove a component from an entity
        template <typename Component>
        void remove(Entity entity) {
            unsigned type_index = registry_.get_id<Component>();
            if(type_index >= pools_.size() ||
               !entities_.is_active(entity)) {
                return;
            }

            ComponentPool<Component> *pool;
            pool = dynamic_cast<ComponentPool<Component> *>(
                pools_[type_index]
            );
            if(pool->search(entity) != -1) {
                pool->remove(entity);
                owned_[entity].erase(type_index);
            }
        };

        // Perform a function on a particular pool of components
        // It must have the following signature: 
        //      void function(Component &c);
        template <typename Component>
        void each(typename fn_sig<Component &>::type func) {
            unsigned type_index = registry_.get_id<Component>();
            if(type_index >= pools_.size() ||
               pools_[type_index] == nullptr) {
                return;
            }
            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                pools_[type_index]
            );
            pool->each(func);
        };

        // Clear a component pool
        template <typename Component>
        void clear() {
            unsigned type_index = registry_.get_id<Component>();
            if(type_index >= pools_.size() ||
               pools_[type_index] == nullptr) {
                return;
            }
            ComponentPool<Component> *pool = dynamic_cast<
                ComponentPool<Component> *>(
                pools_[type_index]
            );
            pool->clear();
        };
    };
}

#endif