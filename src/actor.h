#ifndef DYNAMO_ACTOR_H_
#define DYNAMO_ACTOR_H_

#include "aabb.h"
#include "vec2d.h"

namespace Dynamo {
    // Direction of the actor
    typedef enum {
        UP_DIRECTION,
        DOWN_DIRECTION,
        LEFT_DIRECTION,
        RIGHT_DIRECTION,
        NO_DIRECTION
    } ACTOR_DIRECTION;

    // Physics component
    struct RigidBody {
        AABB hitbox;
        Vec2D vel;

        bool hor_collide;
        bool ver_collide;

        bool solid;
    };

    // Direction component
    struct Direction {
        ACTOR_DIRECTION current;
        ACTOR_DIRECTION prev;
    };

    // Actor represents game objects
    // Can hold state machines for compartmentalizing processes
    class Actor {
        bool alive_;

    public:
        RigidBody body_;
        Direction direction_;

        Actor(AABB hitbox, bool solid);

        // Check if the actor is alive
        bool is_alive();

        // Kill the actor
        void kill();

        // Update the actor
        virtual void update(int dt) = 0;
    };
}

#endif