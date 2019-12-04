#include "actor.h"

namespace Dynamo {
    Actor::Actor(AABB hitbox, bool solid) {
        alive_ = true;
        body_ = {
            hitbox,
            {0, 0},
            false,
            false,
            solid
        };
        direction_ = {
            NO_DIRECTION,
            NO_DIRECTION
        };
    }

    bool Actor::is_alive() {
        return alive_;
    }

    void Actor::kill() {
        alive_ = false;
    }
}