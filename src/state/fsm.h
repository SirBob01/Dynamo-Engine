#ifndef DYNAMO_FSM_H_
#define DYNAMO_FSM_H_

#include "state.h"

namespace Dynamo {
    class FSM {
        State *current_;

    public:
        FSM();
        ~FSM();

        // Get current state
        State *get_current();

        // Add a new state to the stack
        void push_state(State *state);

        // Update the state machine
        void update(unsigned dt);
    };
}

#endif