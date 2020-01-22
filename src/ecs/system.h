#ifndef DYNAMO_SYSTEM_H_
#define DYNAMO_SYSTEM_H_

#include <vector>

#include "entity.h"

namespace Dynamo {
    class Entity;
    class World;

    template <typename ... Component>
    class System {
        std::vector<unsigned> id_filter_;
        std::vector<Entity> registered_;

    protected:
        World *world_;

    public:
        /* TODO: Iterate through world entities 
        and check if they contain components
        listed in id_filter_ */
        System(World *world) {
            world_ = world;
        }

        ~System() {
            world_->unregister_system(this);
        }

        bool is_compatible(Entity entity) {
            return true;
        }

        // Add a new entity to be handled by the system
        void add_entity(Entity entity) {
            if(!world_->is_alive(entity) || !is_compatible(entity)) {
                return;
            }
            registered_.push_back(entity);
        }

        // Remove an entity from the system
        void remove_entity(Entity entity) {
            registered_.erase(
                std::remove(
                    registered_.begin(), 
                    registered_.end(), 
                    entity
                ),
                registered_.end();
            );
        }

        // Update the entity registration list perform the actions
        void update(unsigned dt) {
            for(auto &entity : registered_) {
                if(!world_->is_alive(entity)) {
                    remove_entity(entity);
                }
                tick(dt, entity);
            }
        }

        // User-defined function to perform operations on an entity
        virtual void tick(unsigned dt, Entity entity) = 0;
    };
}

#endif