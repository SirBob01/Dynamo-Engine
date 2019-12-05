#include "state.h"

namespace Dynamo {
    State::State(State *parent) {
        parent_ = parent;
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
    
    void State::set_next(State *state) {
        next_ = state;
    }

    void State::terminate() {
        active_ = false;
    }

    void State::update(int dt) {
        return;
    }
}