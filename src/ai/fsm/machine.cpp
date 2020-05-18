#include "machine.h"

namespace Dynamo::AI {
    StateMachine::StateMachine() {
        current_ = nullptr;
    }

    FiniteState *StateMachine::get_current() {
        return current_;
    }

    void StateMachine::push_state(FiniteState *state) {
        current_ = state;
        current_->on_entry();
    }

    void StateMachine::update(unsigned dt) {
        if(!current_) {
            return;
        }

        current_->on_run(dt);
        FiniteState *next = current_->next_;
        if(next) {
            current_->on_exit();
            delete current_;
            push_state(next);
        }
    }
}