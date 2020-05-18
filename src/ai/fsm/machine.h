#ifndef DYNAMO_AI_STATEMACHINE_H_
#define DYNAMO_AI_STATEMACHINE_H_

#include "state.h"

namespace Dynamo::AI {
    // A simple Finite State Machine
    class StateMachine {
        FiniteState *current_;

    public:
        StateMachine();

        // Get current state
        FiniteState *get_current();

        // Add a new state to the stack
        void push_state(FiniteState *state);

        // Update the state machine
        void update(unsigned dt);
    };
}

#endif