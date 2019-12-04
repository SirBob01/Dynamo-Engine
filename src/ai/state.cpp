#include "state.h"

namespace Dynamo::AI {
    State::State(State *parent, Actor *subject) {
        subject_ = subject;

        parent_ = parent;
        next_ = nullptr;

        active_ = true;
    }

    bool State::is_active() {
        return active_;
    }

    Actor *State::get_subject() {
        return subject_;
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
}