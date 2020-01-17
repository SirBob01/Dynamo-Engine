#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <cstdlib>

#include "entity.h"

namespace Dynamo {
    // Represent an instance of a component
    struct ComponentInstance {
        Entity owner;

        void *data;
        size_t size;
    };

    class ComponentRegistry {
        std::unordered_map<
            Entity, std::unordered_map<std::type_index, ComponentInstance *>
        > entities_;

        std::unordered_map<
            std::type_index, std::vector<ComponentInstance>
        > components_;

    public:
        // Set a component to an entity based on a user-given template
        template<class Component>
        void set_component(Entity entity, Component prefab);

        // Remove a component from an entity
        template<class Component>
        void remove_component(Entity entity);

        // Return a pointer to an entity's component
        template<class Component>
        Component *get_component(Entity entity);
    };
}

#endif