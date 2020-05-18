#ifndef DYNAMO_AI_FINITESTATE_H_
#define DYNAMO_AI_FINITESTATE_H_

namespace Dynamo::AI {
    // Base class for individual Finite States
    class FiniteState {
        FiniteState *next_;

        friend class StateMachine;

    public:
        FiniteState();
        virtual ~FiniteState() = default;

        // Set the next state
        void set_next(FiniteState *next);

        // Process on entry, called once
        virtual void on_entry();

        // Process on exit, called once
        virtual void on_exit();

        // Process over instance lifetime
        virtual void on_run(unsigned dt);
    };
}

#endif