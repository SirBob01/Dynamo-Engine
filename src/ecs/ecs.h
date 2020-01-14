#ifndef DYNAMO_ECS_H_
#define DYNAMO_ECS_H_

#include <vector>
#include <map>

namespace Dynamo {
    // Implicit maximum of 2^32 - 1 possible unique entities
    typedef unsigned long EntityID;

    struct BaseComponent;

    // Keeps track of entities and components
    class EntityManager {
        
    public:
        EntityID create_entity();
        
        void destroy_entity();
    };

    // Base class for systems
    class System {
    public:
        virtual void update(unsigned dt) = 0;
    };
}

#endif