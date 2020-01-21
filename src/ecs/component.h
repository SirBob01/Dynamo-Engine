#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <vector>
#include <algorithm>
#include <cstring>
#include <cinttypes>

#include "entity.h"
#include "../log/error.h"

namespace Dynamo {
    template <typename Component>
    struct ComponentWrapper {
        int entity_index;
        Component *data;
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
        std::vector<uint32_t> dense_;

        Component *pool_;
    
    public:
        ComponentPool() {
            length_ = 0;
            max_value_ = 4;
            capacity_ = 2;

            sparse_ = new uint32_t[max_value_];
            pool_ = static_cast<Component *>(
                std::malloc(sizeof(Component) * capacity_)
            );
        }

        ~ComponentPool() {
            clear();
            std::free(pool_);
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
            return &pool_[index];
        }

        ComponentWrapper<Component> get_pair(int index) {
            if(index < 0 || index >= get_length()) {
                return {-1, nullptr};
            }
            return {dense_[index], &pool_[index]};
        }

        // Search the index of the component instance owned by the entity
        // Returns -1 on failure
        int search(uint32_t entity_id) {
            if(entity_id > max_value_) {
                return -1;
            }
            if(sparse_[entity_id] < get_length() && 
               dense_[sparse_[entity_id]] == entity_id) {
                return sparse_[entity_id];
            }
            return -1;
        }

        // Add a new component
        template <typename ... Fields>
        void insert(Entity entity, Fields ... params) {
            uint32_t entity_id = EntityTracker::get_index(entity);
            if(search(entity_id) != -1) {
                return;
            }
            if(length_+1 >= capacity_) {
                // Reallocate component array
                capacity_ *= 2;
                pool_ = static_cast<Component *>(
                    std::realloc(pool_, sizeof(Component) * capacity_)
                );
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
            dense_.push_back(entity_id);
            pool_[length_++] = {params ...};
        }

        // Destroy a component instance if it exists
        void remove(uint32_t entity_id) {
            int index = search(entity_id);
            if(index < 0) {
                return;
            }

            // Destoroy target and fill its place
            pool_[index].~Component();
            pool_[index] = pool_[get_length()-1];

            // Rearrange indices
            uint32_t temp = dense_.back();
            dense_[index] = temp;
            sparse_[temp] = sparse_[entity_id];
            length_--;
            dense_.pop_back();
        }

        // Clear the pool
        void clear() {
            dense_.clear();
            for(int i = 0; i < length_; i++) {
                pool_[i].~Component();
            }
            length_ = 0;
        }
    };
}

#endif