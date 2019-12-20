#include "fsm.h"

namespace Dynamo {
    FSM::FSM() {
        current_ = nullptr;
    }

    State *FSM::get_current() {
        return current_;
    }

    void FSM::push_state(State *state) {
        current_ = state;
    }

    void FSM::update(unsigned dt) {
        if(!current_) {
            return;
        }
        
        current_->update(dt);
        State *next = current_->get_next();
        State *parent = current_->get_parent();

        current_->set_next(nullptr);
        if(!current_->is_active()) {
            current_->on_exit();
            delete current_;
            current_ = nullptr;
        }

        if(next) {
            if(current_) {
                current_->on_exit();
            }
            if(next != parent) {
                next->set_parent(current_);
            }
            next->on_entry();
            push_state(next);
        }
    }
}