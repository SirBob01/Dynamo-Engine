#ifndef DYNAMO_ECS_H_
#define DYNAMO_ECS_H_

#include <vector>
#include <map>
#include <typeinfo>

namespace Dynamo {
    // Implicit maximum of 2^32 - 1 possible unique entities
    typedef unsigned long EntityID;

    struct EntityDefinition {

    };

    struct BaseComponent {
        EntityID owner;

        BaseComponent(EntityID owner_id) : owner(owner_id);
    };

    template <typename T>
    struct Component : BaseComponent {

    };

    // Keeps track of entities and components
    class EntityManager {
        EntityID entities_;

        // Should map a Component type to its own array
        std::map<std::type_info, std::vector<Component>>

    public:
        EntityManager() : entities_(0);

        EntityID create_entity(EntityDefinition def);

        void destroy_entity();
    };

    // Base class for systems
    class System {
    public:
        virtual void update(unsigned dt) = 0;
    };
}

#endif