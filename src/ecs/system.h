#ifndef DYNAMO_SYSTEM_H_
#define DYNAMO_SYSTEM_H_

#include <vector>
#include <algorithm>

#include "component.h"
#include "world.h"

namespace Dynamo {
    template <typename ... Component>
    class System {
        std::vector<BasePool *> pools_;

    protected:
        World *world_;

    public:
        System(World *world) {
            world_ = world;
            // Grab the pools of each component type
            std::initializer_list<int>{
                ((void)pools_.push_back(
                    world_->get_pool<Component>()
                ), 0)...
            };
        }

        // Perform tick on all entities that match component configuration
        void update(unsigned dt) {
            // Get minimum length pool
            for(auto &pool : pools_) {
                if(pool->get_length() < pools_[0]->get_length()) {
                    std::swap(pools_[0], pool);
                }
            }

            // Loop through entities in min and check if 
            // they exist in the other pools
            BasePool *min = pools_[0];
            for(int i = 0; i < min->get_length(); i++) {
                Entity entity = min->get_entity(i);
                
                bool match = true;
                for(int j = 1; j < pools_.size() && match; j++) {
                    if(pools_[j]->search(entity) == -1) {
                        match = false;
                    }
                }
                if(match) {
                    tick(dt, entity);
                }
            }
        }

        // User-defined function to perform operations on an entity
        virtual void tick(unsigned dt, Entity entity) = 0;
    };
}

#endif