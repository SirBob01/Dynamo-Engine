#ifndef DYNAMO_FSM_H_
#define DYNAMO_FSM_H_

#include <stack>

#include "state.h"

namespace Dynamo {
    class FSM {
        std::stack<State *> state_stack_;
        State *current_;

    public:
        FSM();
        ~FSM();

        // Get current state
        State *get_current();

        // Check if state stack is empty
        bool is_empty();

        // Add a new state to the stack
        void push_state(State *state);

        // Update the state machine
        void update(int dt);
    };
}

#endif