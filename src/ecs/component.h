#ifndef DYNAMO_COMPONENT_H_
#define DYNAMO_COMPONENT_H_

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <typeindex>
#include <cstdlib>

#include "entity.h"
#include "../log/error.h"

namespace Dynamo {
    // Represent an instance of a component
    struct ComponentInstance {
        Entity owner;
        char *data;
        size_t size;

        ~ComponentInstance();
    };

    class ComponentRegistry {
        // Access by entity
        std::unordered_map<
            Entity, std::unordered_map<std::type_index, ComponentInstance *>
        > entities_;

        // Access by type
        std::unordered_map<
            std::type_index, std::vector<ComponentInstance>
        > components_;

    public:
        std::unordered_map<
            std::type_index, ComponentInstance *
        > &get_collection(Entity entity);
        
        // Return a pointer to an entity's component
        template<typename Component>
        Component *get_component(Entity entity);

        // Set a component to an entity based on a user-given template
        template<typename Component>
        void set_component(Entity entity, Component prefab);

        // Remove a component from an entity
        template<typename Component>
        void remove_component(Entity entity);

        // Remove all components of an entity
        void destroy_entity(Entity entity);
    };
}

#endif