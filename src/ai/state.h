#ifndef DYNAMO_AI_STATE_H_
#define DYNAMO_AI_STATE_H_

#include "../actor.h"

namespace Dynamo::AI {
    // AI state for actors
    class State {
        Actor *subject_;

        State *parent_;
        State *next_;

        bool active_;

    public:
        State(State *parent, Actor *subject);

        // Check if the state is active
        bool is_active();

        // Get the handler of this state
        Actor *get_subject();

        // Get the next state
        State *get_next();

        // Set the next state
        void set_next(State *state);

        // Terminate this process
        void terminate();

        // Process on instance creation
        virtual void on_entry() = 0;

        // Process on instance deletion
        virtual void on_exit() = 0;

        // Process over instance lifetime
        virtual void update(int dt) = 0;
    };
}

#endif