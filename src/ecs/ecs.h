#ifndef DYNAMO_ECS_H_
#define DYNAMO_ECS_H_

#include <vector>
#include <tuple>
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

        // Test if all the a type has no corresponding pool
        template <typename... T>
        bool has_invalid_pool() {
            std::vector<int> pool_ids {
                static_cast<int>(registry_.get_id<T>())...
            };
            for(auto &id : pool_ids) {
                if(id >= pools_.size() || 
                   pools_[id] == nullptr) {
                    return true;
                }
            }
            return false;
        }

        // Grab a component from an entity
        // Skip the unnecessary checks for faster cycles
        template <typename Type, typename Min, class Container=Pool<Type>>
        inline Type *fast_grab(Container *pool, Entity entity, int index) {
            if constexpr(std::is_same_v<Type, Min>) {
                // No need for indirection with smallest container
                // Cache friendly! :)
                return pool->get_at(index);
            }
            else {
                return pool->get_from_entity(entity);
            }
        };

        // Perform func on entities found in the group,
        // but not the ones found in the excluded pools
        template <typename Min, typename Func, 
                  typename... Type, template <typename...> class T=TypeList,
                  typename... Expt, template <typename...> class E=TypeList>
        void iterate_group(Func &func, BasePool *min, auto &pools,
                           T<Type...> &, E<Expt...> &) {
            int min_index = 0;
            for(auto &entity : min->get_members()) {
                if((std::get<Pool<Type> *>(pools)->exists(entity) &&...) &&
                  (!std::get<Pool<Expt> *>(pools)->exists(entity) &&...)) {
                    func(
                        entity, 
                        *fast_grab<Type, Min>(
                            std::get<Pool<Type> *>(pools), entity,
                            min_index
                        )...
                    );
                }
                min_index++;
            }
        }

    public:
        ~ECS();

        // Create an new entity
        Entity create_entity();

        // Destroy an entity
        void destroy_entity(Entity entity);

        // Perform a function on a particular pool of components
        // It must have the following signature: 
        //      void function(Type &c);
        template <typename Type, typename Func>
        void each(Func func) {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size() ||
               pools_[type_index] == nullptr) {
                return;
            }
            Pool<Type> *pool;
            pool = static_cast<Pool<Type> *>(
                pools_[type_index]
            );
            pool->each(func);
        };

        // Perform a function on entities within a component group
        template <typename... Type, typename... Expt,
                  template <typename...> class List=TypeList,
                  typename Func>
        void each_group(Func func, const List<Expt...> &exclude=TypeList<>{}) {
            // Iterate through the smallest pool for optimal speed
            if(has_invalid_pool<Type..., Expt...>()) {
                return;
            }

            // Accessing tuple is much faster than accessing vector
            auto pools = std::make_tuple(
                static_cast<Pool<Type> *>(pools_[registry_.get_id<Type>()])...,
                static_cast<Pool<Expt> *>(pools_[registry_.get_id<Expt>()])...
            );
            auto min = std::min(
                {static_cast<BasePool *>(std::get<Pool<Type> *>(pools))...}, 
                [](auto &a, auto &b) {
                    return a->get_length() < b->get_length();
                }
            );

            TypeList<Type...> members_list;
            TypeList<Expt...> exclude_list;
            ((std::get<Pool<Type> *>(pools) == min ? 
                iterate_group<Type>(
                    func, min, pools,
                    members_list, exclude_list
                ) : void()),...
            );
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
            
            Pool<Type> *pool;
            pool = static_cast<Pool<Type> *>(
                pools_[type_index]
            );
            int pos = pool->search(entity);
            if(pos == -1) {
                return nullptr;
            }
            return pool->get_at(pos);
        };

        // Add a component to an entity
        template <typename Type, typename... Fields>
        void assign(Entity entity, Fields... params) {
            unsigned type_index = registry_.get_id<Type>();
            if(type_index >= pools_.size()) {
                pools_.resize(type_index + 1, nullptr);
            }
            if(pools_[type_index] == nullptr) {
                pools_[type_index] = new Pool<Type>();   
            }
            if(!entities_.is_active(entity)) {
                return;
            }
            Pool<Type> *pool;
            pool = static_cast<Pool<Type> *>(
                pools_[type_index]
            );
            if(pool->search(entity) == -1) {
                pool->insert(entity, params...);
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
            Pool<Type> *pool;
            pool = static_cast<Pool<Type> *>(
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
            Pool<Type> *pool;
            pool = static_cast<Pool<Type> *>(
                pools_[type_index]
            );
            pool->clear();
        };

        void clear_all();
    };
}

#endif