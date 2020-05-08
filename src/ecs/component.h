#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <vector>
#include <algorithm>
#include <cinttypes>

#include "entity.h"
#include "../log/error.h"

namespace Dynamo {
    // Dynamic sparse set for Type data
    // Cache friendly storage of components and fast search-by-Entity
    // Each Entity in dense_ directly maps to Type in pool_
    //  Search: O(1)
    //  Remove: O(1)
    //  Insert: O(1) ammortized
    class BasePool {
    protected:
        std::vector<int> sparse_;       // Indices to dense_ and pool_
        std::vector<Entity> dense_;     // Indices to sparse_
        
    public:
        virtual ~BasePool() = default;

        // Get the length of the sparse set
        inline int get_length() {
            return dense_.size();
        };

        // Get the entity at an index
        inline Entity get_entity(int index) {
            return dense_[index];
        };

        // Get the entities associated with the component
        inline std::vector<Entity> &get_members() {
            return dense_;
        };

        // Search the index of the component instance owned by the entity
        // Returns -1 on failure
        inline int search(Entity entity) {
            uint32_t entity_index = EntityTracker::get_index(entity);
            if(entity_index >= sparse_.size()) {
                return -1;
            }
            if(sparse_[entity_index] != -1 && 
               dense_[sparse_[entity_index]] == entity) {
                return sparse_[entity_index];
            }
            return -1;
        };

        // Check if an entity exists within a container
        inline bool exists(Entity entity) {
            return search(entity) != -1; 
        };

        // Remove an entity from the pool
        // Implemented in derived template pools
        // as it involves component destruction
        virtual void remove(Entity entity) = 0;
    };

    // Specialized pool for each Component
    // Necessary for creation and destruction of objects
    template <typename Type>
    class Pool : public BasePool {
        std::vector<Type> pool_;    // Holds the components objects

    public:
        // Get the pointer to a component at an index
        inline Type *get_at(int index) {
            return &pool_[index];
        };

        // Get the pointer to a component from an Entity ID
        // Skip indirection of searching for index from entity
        // Assumes that the entity exists in the set
        inline Type *get_from_entity(Entity entity) {
            return &pool_[sparse_[EntityTracker::get_index(entity)]];
        };

        // Add a new component
        // If the component is constructible, call the constructor
        // Otherwise, aggregate-initialize the object
        template <typename ... Fields>
        inline void insert(Entity entity, Fields ... params) {
            uint32_t entity_index = EntityTracker::get_index(entity);
            if(entity_index >= sparse_.size()) {
                // Reallocate sparse array
                sparse_.resize((entity_index + 1) * 2, -1);
            }
            if(search(entity) != -1) {
                return;
            }

            sparse_[entity_index] = dense_.size();
            dense_.emplace_back(entity);
            
            if(std::is_aggregate_v<Type>) {
                pool_.push_back(Type {params ...});            
            }
            else {
                pool_.emplace_back(params ...);
            }
        };

        // Destroy a component instance
        inline void remove(Entity entity) {
            int index = search(entity);
            if(index == -1) {
                return;
            }

            // Destroy target and fill its place
            std::swap(pool_[index], pool_.back());
            pool_.pop_back();

            Entity temp = dense_.back();
            dense_[index] = temp;
            dense_.pop_back();

            // Rearrange sparse set indices            
            uint32_t entity_index = EntityTracker::get_index(entity);
            uint32_t temp_index = EntityTracker::get_index(temp);
            sparse_[temp_index] = sparse_[entity_index];
        };

        // Perform a function on each component
        template <class F>
        void each(F function) {
            for(auto &component : pool_) {
                function(component);
            }
        };

        // Clear the pool
        void clear() {
            pool_.clear();
            dense_.clear();
        };
    };
}

#endif