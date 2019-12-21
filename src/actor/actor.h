#ifndef DYNAMO_ACTOR_H_
#define DYNAMO_ACTOR_H_

#include <unordered_map>
#include <string>

#include "../core/textures.h"
#include "../core/jukebox.h"
#include "../sprite/sprite.h"
#include "../state/fsm.h"
#include "../state/state.h"
#include "../util/aabb.h"
#include "../util/vec2d.h"
#include "../log/error.h"

namespace Dynamo {
    // Collision flags
    struct Collision {
        bool top;
        bool bottom;
        bool left;
        bool right;

        void reset();
    };

    // Physics component
    struct RigidBody {
        AABB hitbox;
        Vec2D vel;
        Collision collision;

        bool solid;
    };

    // Resource managers
    struct Resources {
        TextureManager *textures;
        Jukebox *jukebox;
    };

    // Actor represents game objects
    class Actor {
    protected:
        TextureManager *textures_;
        Jukebox *jukebox_;
        
        bool alive_;
        RigidBody body_;

        Sprite *sprite_;

        FSM *brain_;
        std::unordered_map<std::string, Dynamo::State *> states_;
        
    public:
        Actor(AABB hitbox, bool solid, Resources resources);
        ~Actor();

        // Check if the actor is alive
        bool is_alive();

        // Get a reference to the physical body of the actor
        RigidBody &get_body();

        // Get the sprite of the actor
        Sprite *get_sprite();

        // Get a possible state of the actor
        State *get_state(std::string state_id);

        // Set the sprite of the actor
        void set_sprite(Sprite *sprite);

        // Set the state of the actor
        void set_state(std::string state_id);

        // Kill the actor
        void kill();

        // Update the brain
        void think(unsigned dt);

        // Update the actor
        virtual void update(unsigned dt) = 0;
    };
}

#endif