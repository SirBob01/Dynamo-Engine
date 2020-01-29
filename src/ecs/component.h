#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <vector>
#include <algorithm>
#include <cstring>
#include <cinttypes>

#include "entity.h"
#include "../log/error.h"

namespace Dynamo {
    // Common base-class for templated ComponentPools
    class BasePool {
    public:
        virtual ~BasePool() = default;

        virtual int get_length() = 0;

        virtual int search(Entity entity) = 0;

        virtual Entity get_entity(int index) = 0;

        virtual void remove(Entity entity) = 0;

        virtual void clear() = 0;
    };

    // Dynamic sparse set of component data
    // Allows quick iteration and search-by-Entity
    template <typename Component>
    class ComponentPool : public BasePool {
        int length_;
        int capacity_;
        uint32_t max_value_;

        uint32_t *sparse_; // Holdes indices to dense_ and pool_
        std::vector<Entity> dense_; // Holds indices to sparse_

        Component *pool_;
    
    public:
        ComponentPool() {
            length_ = 0;
            capacity_ = 2;
            max_value_ = 4;

            sparse_ = new uint32_t[max_value_];
            pool_ = static_cast<Component *>(
                std::malloc(sizeof(Component) * capacity_)
            );
        };

        ~ComponentPool() {
            clear();
            std::free(pool_);
            delete[] sparse_;
        };

        // Get the length of the sparse set
        int get_length() {
            return length_;
        };

        // Get a pointer to a component instance 
        // Use for iteration
        Component *get_at(int index) {
            return pool_ + index;
        };

        // Get the entity at an index
        Entity get_entity(int index) {
            return dense_[index];
        };

        // Search the index of the component instance owned by the entity
        // Returns -1 on failure
        int search(Entity entity) {
            uint32_t entity_index = EntityTracker::get_index(entity);
            if(entity_index > max_value_) {
                return -1;
            }
            if(sparse_[entity_index] < get_length() &&
               dense_[sparse_[entity_index]] == entity) {
                return sparse_[entity_index];
            }
            return -1;
        };

        // Add a new component
        template <typename ... Fields>
        void insert(Entity entity, Fields ... params) {
            uint32_t entity_index = EntityTracker::get_index(entity);
            if(search(entity) != -1) {
                return;
            }
            if(length_+1 >= capacity_) {
                // Reallocate component array
                capacity_ *= 2;
                pool_ = static_cast<Component *>(
                    std::realloc(pool_, sizeof(Component) * capacity_)
                );
            }
            if(entity_index >= max_value_) {
                // Reallocate sparse array
                uint32_t *new_sparse = new uint32_t[entity_index * 2];
                for(int i = 0; i < max_value_; i++) {
                    new_sparse[i] = sparse_[i];
                }

                delete[] sparse_;
                sparse_ = new_sparse;
                max_value_ = entity_index * 2;
            }

            sparse_[entity_index] = get_length();
            dense_.push_back(entity);
            pool_[length_++] = {params ...};
        };

        // Destroy a component instance if it exists
        void remove(Entity entity) {
            int index = search(entity);
            if(index < 0) {
                return;
            }

            // Destoroy target and fill its place
            pool_[index].~Component();
            pool_[index] = pool_[get_length()-1];

            // Rearrange indices
            Entity temp = dense_.back();
            uint32_t entity_index = EntityTracker::get_index(entity);
            uint32_t temp_index = EntityTracker::get_index(temp);

            dense_[index] = temp;
            sparse_[temp_index] = sparse_[entity_index];
            length_--;
            dense_.pop_back();
        };

        // Perform a function on each component
        template <class F>
        void each(F function) {
            for(int i = 0; i < length_; i++) {
                function(pool_ + i);
            }
        };

        // Clear the pool
        void clear() {
            dense_.clear();
            for(int i = 0; i < get_length(); i++) {
                pool_[i].~Component();
            }
            length_ = 0;
        };
    };
}

#endif