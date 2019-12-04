#include "fsm.h"

namespace Dynamo::AI {
    void FSM::push_state(State *state) {
        state->on_entry();
        state_stack_.push(state);
    }

    void FSM::update(int dt) {
        // Similar logic with scene engine
        if(!state_stack_.empty()) {
            State *current = state_stack_.top();
            current->update(dt);

            State *next = current->get_next();
            if(!current->is_active()) {
                current->on_exit();
                delete current;
                current = nullptr;
                state_stack_.pop();

                if(next) {
                    push_state(next);
                }
            }
        }
    }
}