#include "actor.h"

namespace Dynamo {
    Actor::Actor(AABB hitbox, bool solid) {
        alive_ = true;
        
        direction_ = {
            NO_DIRECTION,
            NO_DIRECTION
        };

        body_ = {
            hitbox,
            {0, 0},
            false,
            false,
            solid
        };
    }

    bool Actor::is_alive() {
        return alive_;
    }

    ACTOR_DIRECTION Actor::get_direction() {
        return direction_.current;
    }

    void Actor::set_direction(ACTOR_DIRECTION direction) {
        direction_.prev = direction_.current;
        direction_.current = direction;
    }

    void Actor::kill() {
        alive_ = false;
    }
}