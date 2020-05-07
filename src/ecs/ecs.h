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

        // Function signature for working with components
        template <typename ... T>
        struct fn_sig {
            typedef std::function<void(T ...)> type;
        };

        // Grab a component from an entity
        // Skip the unnecessary checks for faster cycles
        template <typename Type>
        inline Type *fast_grab(Entity entity) {
            return static_cast<ComponentPool<Type> *>(
                pools_[registry_.get_id<Type>()]
            )->get_from_entity(entity);
        };

    public:
        template <typename ... Type>
        struct exclude {};

        ~ECS();

        // Create an new entity
        Entity create_entity();

        // Destroy an entity
        void destroy_entity(Entity entity);

        // Perform a function on a particular pool of components
        // It must have the following signature: 
        //      void function(Type &c);
        template <typename Type>
        void each(typename fn_sig<Type &>::type func) {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size() ||
               pools_[type_index] == nullptr) {
                return;
            }
            ComponentPool<Type> *pool;
            pool = static_cast<ComponentPool<Type> *>(
                pools_[type_index]
            );
            pool->each(func);
        };

        // Perform a function on entities within a component group
        template <typename ... Type, typename ... Expt, 
                  template <typename ...> class List=exclude>
        void each_group(typename fn_sig<Entity &, Type &...>::type func,
                        const List<Expt ...> &ex_list=exclude<>{}) {
            // Iterate through the smallest pool for optimal speed
            std::vector<int> pool_ids {
                static_cast<int>(registry_.get_id<Type>()) ...
            };
            for(auto &id : pool_ids) {
                if(id >= pools_.size() || 
                   pools_[id] == nullptr) {
                    return;
                }
            }
            auto min_iter = std::min_element(
                pool_ids.begin(), pool_ids.end(), [this](auto &a, auto &b) {
                    return pools_[a]->get_length() < pools_[b]->get_length();
                }
            );

            // Perform func on entities found in the group,
            // but not the ones found in the excluded pools
            for(auto &entity : pools_[*min_iter]->get_members()) {
                if((pools_[registry_.get_id<Type>()]->exists(entity) && ...) &&
                  (!pools_[registry_.get_id<Expt>()]->exists(entity) && ...)) {
                    func(entity, *fast_grab<Type>(entity) ...);
                }
            }
        };

        // Get the pointer to an entity's component if it exists
        template <typename Type>
        Type *grab(Entity entity) {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size() || 
               pools_[type_index] == nullptr || 
               !entities_.is_active(entity)) {
                return nullptr;
            }
            ComponentPool<Type> *pool;
            pool = static_cast<ComponentPool<Type> *>(
                pools_[type_index]
            );
            return pool->get_at(pool->search(entity));
        };

        // Add a component to an entity
        template <typename Type, typename ... Fields>
        void assign(Entity entity, Fields ... params) {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size()) {
                pools_.resize(type_index + 1, nullptr);
            }
            if(pools_[type_index] == nullptr) {
                pools_[type_index] = new ComponentPool<Type>();   
            }
            if(!entities_.is_active(entity)) {
                return;
            }
            ComponentPool<Type> *pool;
            pool = static_cast<ComponentPool<Type> *>(
                pools_[type_index]
            );
            if(pool->search(entity) == -1) {
                pool->insert(entity, params ...);
                owned_[entity][type_index] = 1;
            }
        };

        // Remove a component from an entity
        template <typename Type>
        void remove(Entity entity) {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size() ||
               !entities_.is_active(entity)) {
                return;
            }
            ComponentPool<Type> *pool;
            pool = static_cast<ComponentPool<Type> *>(
                pools_[type_index]
            );
            if(pool->search(entity) != -1) {
                pool->remove(entity);
                owned_[entity].erase(type_index);
            }
        };

        // Clear a component pool
        template <typename Type>
        void clear() {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size() ||
               pools_[type_index] == nullptr) {
                return;
            }
            ComponentPool<Type> *pool;
            pool = static_cast<ComponentPool<Type> *>(
                pools_[type_index]
            );
            pool->clear();
        };

        void clear_all();
    };
}

#endif