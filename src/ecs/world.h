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
    class World {
        EntityTracker entities_;
        ComponentRegistry registry_;

        std::vector<std::type_index> system_types_;
        std::vector<System *> systems_;

    public:
        ~World();

        Entity create_entity();

        void kill_entity(Entity entity);

        template <class Component>
        void add_component(Entity entity, Component prefab);

        template <class S>
        void register_system();

        void update(unsigned dt);
    };
}

#endif