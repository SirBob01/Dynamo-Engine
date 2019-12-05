#include "fsm.h"

namespace Dynamo {
    FSM::FSM() {
        current_ = nullptr;
    }

    FSM::~FSM() {
        if(current_) {
            current_->destroy_ancestors();        
            delete current_;
        }
    }

    State *FSM::get_current() {
        return current_;
    }

    void FSM::push_state(State *state) {
        current_ = state;
    }

    void FSM::update(int dt) {
        if(current_) {
            current_->update(dt);
            State *next = current_->get_next();
            State *parent = current_->get_parent();
            current_->set_next(nullptr);

            if(!current_->is_active()) {
                delete current_;
                current_ = nullptr;
            }

            if(next) {
                if(next != parent) {
                    next->set_parent(current_);
                }
                push_state(next);
            }
        }
    }
}