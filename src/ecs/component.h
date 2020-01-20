#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <vector>
#include <cstring>
#include <cinttypes>

#include "entity.h"
#include "../log/error.h"

namespace Dynamo {
    template <typename Component>
    struct ComponentHandle {
        Entity owner;
        Component data;
    };

    // Static methods for generating unique type IDs
    class TypeID {
        static unsigned increment_id();

    public:
        template <typename T>
        static unsigned get_id() {
            static const unsigned id = increment_id();
            return id;
        }
    };

    class BasePool {
    public:
        virtual ~BasePool() = default;

        virtual void remove(uint32_t entity_id) = 0;

        virtual void clear() = 0;
    };

    // Dynamic sparse set of component data
    // Allows quick iteration and search-by-Entity
    template <typename Component>
    class ComponentPool : public BasePool {
        uint32_t length_;
        uint32_t max_value_;
        uint32_t capacity_;

        uint32_t *sparse_;
        ComponentHandle<Component> *dense_;
    
    public:
        ComponentPool() {
            length_ = 0;
            max_value_ = 4;
            capacity_ = 2;

            sparse_ = new uint32_t[max_value_];
            dense_ = new ComponentHandle<Component>[capacity_];
        }

        ~ComponentPool() {
            delete[] dense_;
            delete[] sparse_;
        }

        // Get the length of the sparse set
        int get_length() {
            return length_;
        }

        // Get a pointer to a component instance 
        // Use for iteration
        Component *get_at(int index) {        
            if(index < 0 || index >= get_length()) {
                return nullptr;
            }
            return &dense_[index].data;
        }

        // Search the index of the component instance owned by the entity
        // Returns -1 on failure
        int search(uint32_t entity_id) {
            if(entity_id > max_value_) {
                return -1;
            }
            if(sparse_[entity_id] < get_length()) {
                uint32_t index = EntityTracker::get_index(
                    dense_[sparse_[entity_id]].owner
                );
                if(index == entity_id) {
                    return sparse_[entity_id];
                }
            }
            return -1;
        }

        // Add a new component
        void insert(Entity entity, Component prefab) {
            uint32_t entity_id = EntityTracker::get_index(entity);
            if(search(entity_id) != -1) {
                return;
            }
            if(length_+1 >= capacity_) {
                // Reallocate dense array
                capacity_ *= 2;
                ComponentHandle<Component> *new_dense;
                new_dense = new ComponentHandle<Component>[capacity_];
                for(int i = 0; i < length_; i++) {
                    new_dense[i] = dense_[i];
                }

                delete[] dense_;
                dense_ = new_dense;
            }
            if(entity_id >= max_value_) {
                // Reallocate sparse array
                uint32_t *new_sparse = new uint32_t[entity_id * 2];
                for(int i = 0; i < max_value_; i++) {
                    new_sparse[i] = sparse_[i];
                }

                delete[] sparse_;
                sparse_ = new_sparse;
                max_value_ = entity_id * 2;
            }

            sparse_[entity_id] = get_length();
            dense_[length_++] = {entity, prefab};
        }

        // Destroy a component instance if it exists
        void remove(uint32_t entity_id) {
            int index = search(entity_id);
            if(index < 0) {
                return;
            }

            // Perform the swap to remove holes
            ComponentHandle<Component> temp = dense_[get_length()-1];
            dense_[index] = temp;

            uint32_t entity_index = EntityTracker::get_index(temp.owner);
            sparse_[entity_index] = sparse_[entity_id];
            length_--;
        }

        // Clear the pool
        void clear() {
            length_ = 0;
        }
    };
}

#endif