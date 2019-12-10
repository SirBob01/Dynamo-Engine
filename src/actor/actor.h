#ifndef DYNAMO_ACTOR_H_
#define DYNAMO_ACTOR_H_

#include "../sprite/sprite.h"
#include "../state/fsm.h"
#include "../state/state.h"
#include "../util/aabb.h"
#include "../util/vec2d.h"

namespace Dynamo {
    // Collision flags
    struct Collision {
        bool top;
        bool bottom;
        bool left;
        bool right;
    };

    // Physics component
    struct RigidBody {
        AABB hitbox;
        Vec2D vel;
        Collision collision;

        bool solid;
    };

    // Actor represents game objects
    class Actor {
    protected:
        bool alive_;
        RigidBody body_;
        FSM *brain_;
        Sprite *sprite_;

    public:
        Actor(AABB hitbox, bool solid);
        ~Actor();

        // Check if the actor is alive
        bool is_alive();

        // Get a reference to the physical body of the actor
        RigidBody &get_body();

        // Get the sprite of the actor
        Sprite *get_sprite();

        // Set the state of the actor
        void push_state(State *state);

        // Set the sprite of the actor
        void set_sprite(Sprite *sprite);

        // Kill the actor
        void kill();

        // Update the brain
        void think(unsigned dt);

        // Update the actor
        virtual void update(unsigned dt) = 0;
    };
}

#endif