#include "state.h"

namespace Dynamo {
    State::State() {
        parent_ = nullptr;
        next_ = nullptr;

        active_ = true;
    }

    State::~State() {
        return;
    }

    bool State::is_active() {
        return active_;
    }

    State *State::get_parent() {
        return parent_;
    }

    State *State::get_next() {
        return next_;
    }
    
    void State::set_parent(State *state) {
        parent_ = state;
    }

    void State::set_next(State *state) {
        next_ = state;
    }

    void State::terminate() {
        active_ = false;
    }

    void State::destroy_ancestors() {
        State *next;
        while(parent_) {
            next = parent_->get_parent();
            delete parent_;
            parent_ = next;
        }
    }

    void State::update(unsigned dt) {
        return;
    }
}