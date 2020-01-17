#ifndef DYNAMO_SYSTEM_H_
#define DYNAMO_SYSTEM_H_

#include <vector>
#include <algorithm>

#include "entity.h"
#include "component.h"

namespace Dynamo {
    class System {
        std::vector<Entity> entities_;

    protected:
        ComponentRegistry *registry_;

    public:
        System(ComponentRegistry *component_registry);

        // Registry an entity to be targeted by the system
        void register_entity(Entity entity);

        // Remove an entity from the registry
        void unregister_entity(Entity entity);

        // Loop through entities and apply tick method
        void update(unsigned dt);

        // Perform actual game logic on registered entities
        virtual void tick(Entity entity, unsigned dt) = 0;
    };
}

#endif