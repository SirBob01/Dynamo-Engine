#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <vector>
#include <algorithm>
#include <cinttypes>

#include "entity.h"
#include "../log/error.h"

namespace Dynamo {
    // Common base-class for templated ComponentPools
    // Allows quick iteration and search-by-Entity
    //  Search: O(1)
    //  Remove: O(1)
    //  Insert: O(1) ammortized
    class BasePool {
    protected:
        std::vector<int> sparse_;   // Holds indices to dense_ and pool_
        std::vector<Entity> dense_; // Holds indices to sparse_

        uint32_t max_value_;        // Maximum Entity value (size of dense_)
        
    public:
        BasePool() {
            max_value_ = 4;
            sparse_.resize(max_value_, -1);
        };

        // Get the length of the sparse set
        inline int get_length() {
            return dense_.size();
        }

        // Get the entity at an index
        inline Entity get_entity(int index) {
            return dense_[index];
        };

        // Get the entities associated with the component
        inline std::vector<Entity> &get_members() {
            return dense_;
        }

        // Search the index of the component instance owned by the entity
        // Returns -1 on failure
        inline int search(Entity entity) {
            uint32_t entity_index = EntityTracker::get_index(entity);
            if(entity_index >= max_value_) {
                return -1;
            }
            if(sparse_[entity_index] != -1 && 
               dense_[sparse_[entity_index]] == entity) {
                return sparse_[entity_index];
            }
            return -1;
        };

        virtual void remove(Entity entity) = 0;
    };

    // Dynamic sparse set of component data
    template <typename Component>
    class ComponentPool : public BasePool {
        std::vector<Component> pool_;
    
    public:
        // Compile-time conditional for instantiating components
        // If the component is constructible, call the constructor
        // Otherwise, aggregate-initialize the object
        template<typename ... Fields> 
        std::enable_if_t<std::is_constructible<Component, Fields ...>::value> 
        inline initialize_component(Fields ... params) {
            pool_.emplace_back(params ...);
        };
        template<typename ... Fields> 
        std::enable_if_t<!std::is_constructible<Component, Fields ...>::value> 
        inline initialize_component(Fields ... params) {
            pool_.push_back({params ...});
        };

        // Get a pointer to a component instance 
        // Use for iteration
        inline Component *get_at(int index) {
            return &pool_[index];
        };

        // Perform a function on each component
        template <class F>
        inline void each(F function) {
            for(auto &c : pool_) {
                function(c);
            }
        };

        // Clear the pool
        inline void clear() {
            dense_.clear();
            pool_.clear();
        };

        // Add a new component
        template <typename ... Fields>
        inline void insert(Entity entity, Fields ... params) {
            uint32_t entity_index = EntityTracker::get_index(entity);
            if(entity_index >= max_value_) {
                // Reallocate sparse array
                max_value_ = entity_index * 2;
                sparse_.resize(max_value_, -1);
            }

            sparse_[entity_index] = get_length();
            dense_.push_back(entity);

            initialize_component(params ...);
        };

        // Destroy a component instance
        inline void remove(Entity entity) {
            int index = search(entity);

            // Destroy target and fill its place
            std::swap(pool_[index], pool_.back());
            pool_.pop_back();

            // Rearrange indices
            Entity temp = dense_.back();
            uint32_t entity_index = EntityTracker::get_index(entity);
            uint32_t temp_index = EntityTracker::get_index(temp);

            dense_[index] = temp;
            sparse_[temp_index] = sparse_[entity_index];
            dense_.pop_back();
        };
    };
}

#endif