#ifndef DYNAMO_AI_FSM_H_
#define DYNAMO_AI_FSM_H_

#include <stack>

#include "state.h"

namespace Dynamo::AI {
    class FSM {
        std::stack<State *> state_stack_;

    public:
        // Add a new state to the stack
        void push_state(State *state);

        // Update the state machine
        void update(int dt);
    };
}

#endif