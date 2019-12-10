#include "actor.h"

namespace Dynamo {
    Actor::Actor(AABB hitbox, bool solid) {
        alive_ = true;
        body_ = {
            hitbox,
            {0, 0},
            {false, false, false, false},
            
            solid
        };
        brain_ = new FSM();
        sprite_ = nullptr;
    }

    Actor::~Actor() {
        delete brain_;
    }

    bool Actor::is_alive() {
        return alive_;
    }

    RigidBody &Actor::get_body() {
        return body_;
    }

    Sprite *Actor::get_sprite() {
        return sprite_;
    }

    void Actor::push_state(State *state) {
        brain_->push_state(state);
    }

    void Actor::set_sprite(Sprite *sprite) {
        sprite_ = sprite;
    }

    void Actor::kill() {
        alive_ = false;
    }

    void Actor::think(unsigned dt) {
        brain_->update(dt);
    }
}