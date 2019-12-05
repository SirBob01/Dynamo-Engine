#include "fsm.h"

namespace Dynamo {
    FSM::FSM() {
        current_ = nullptr;
    }

    FSM::~FSM() {
        while(!is_empty()) {
            state_stack_.pop();
        }
    }

    State *FSM::get_current() {
        return current_;
    }

    bool FSM::is_empty() {
        return state_stack_.empty();
    }

    void FSM::push_state(State *state) {
        state_stack_.push(state);
        current_ = state;
    }

    void FSM::update(int dt) {
        if(current_) {
            current_->update(dt);
            State *next = current_->get_next();
            current_->set_next(nullptr);

            if(!current_->is_active()) {
                state_stack_.pop();
                current_ = nullptr;
            }

            if(next) {
                push_state(next);
            }
        }
        else if(!is_empty()) {
            current_ = state_stack_.top();
        }
    }
}