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

    protected:
        Direction direction_;

    public:
        RigidBody body_;

        Actor(AABB hitbox, bool solid);

        // Check if the actor is alive
        bool is_alive();

        // Get the current direction of the actor
        ACTOR_DIRECTION get_direction();

        // Set the current direction of the actor
        void set_direction(ACTOR_DIRECTION direction);

        // Kill the actor
        void kill();

        // Update the actor
        virtual void update(int dt) = 0;
    };
}

#endif