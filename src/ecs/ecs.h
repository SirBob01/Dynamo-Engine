#ifndef DYNAMO_ECS_H_
#define DYNAMO_ECS_H_

#include <vector>
#include <map>
#include <typeinfo>

namespace Dynamo {
    // Forward declaration
    class EventManager;

    // Implicit maximum of 2^32 - 1 possible unique entities
    typedef unsigned long EntityID;

    struct EntityDefinition {

    };

    struct Component {
        EntityID owner;

        Component(EntityID owner_id) : owner(owner_id);
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
        EventManager *event_manager_;

    public:
        void handle_event(Event event, unsigned dt);
        
        virtual void update(unsigned dt) = 0;
    };
}

#endif