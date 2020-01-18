#ifndef DYNAMO_WORLD_H_
#define DYNAMO_WORLD_H_

#include <vector>
#include <algorithm>
#include <typeinfo>
#include <typeindex>

#include "entity.h"
#include "component.h"
#include "system.h"

namespace Dynamo {
    // TODO: Move ComponentRegistry methods to World
    // Component responsibilities will be delegated to world
    class World {
        IDTracker entities_;
        ComponentRegistry components_;

        std::vector<std::type_index> system_types_;
        std::vector<System *> systems_;

    public:
        ~World();

        Entity create_entity();

        void kill_entity(Entity entity);

        template <typename Component>
        void add_component(Entity entity, Component prefab);

        template <typename Component>
        void remove_component(Entity entity);

        template <class S>
        void register_system();

        void update(unsigned dt);
    };
}

#endif