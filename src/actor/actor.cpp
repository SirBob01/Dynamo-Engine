#include "actor.h"

namespace Dynamo {
    void Collision::reset() {
        top = false;
        bottom = false;
        left = false;
        right = false;
    }

    Actor::Actor(AABB hitbox, bool solid, Resources resources) {
        textures_ = resources.textures;
        jukebox_ = resources.jukebox;
        
        alive_ = true;
        body_ = {
            hitbox,
            {0, 0},
            {0, 0},
            {false, false, false, false},
            
            solid
        };

        sprite_ = nullptr;
        brain_ = new FSM();
    }

    Actor::~Actor() {
        for(auto &state : states_) {
            delete state.second;
        }
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

    State *Actor::get_state(std::string state_id) {
        if(!states_.count(state_id)) {
            throw InvalidKey(state_id, "states_");
        }
        return states_[state_id];
    }

    void Actor::set_sprite(Sprite *sprite) {
        if(sprite_ == sprite) {
            return;
        }
        if(sprite_) {
            sprite_->clear();
            sprite_->set_frame(0);
        }
        sprite_ = sprite;
    }

    void Actor::set_state(std::string state_id) {
        State *current = brain_->get_current();
        State *next = get_state(state_id);
        if(!next) {
            return; // Null states not allowed!
        }
        if(current) {
            if(!current->get_next()) {
                current->set_next(next);            
            }
        }
        else {
            brain_->push_state(next);
        }
    }

    void Actor::kill() {
        alive_ = false;
    }

    void Actor::think(unsigned dt) {
        brain_->update(dt);
    }
}