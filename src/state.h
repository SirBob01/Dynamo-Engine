#ifndef DYNAMO_STATE_H_
#define DYNAMO_STATE_H_

namespace Dynamo {
    class State {
    protected:
        State *parent_;
        State *next_;

        bool active_;

    public:
        State(State *parent);
        virtual ~State();

        // Check if the state is active
        bool is_active();

        // Get previous state
        State *get_parent();

        // Get the next state
        State *get_next();

        // Set the next state
        void set_next(State *state);

        // Terminate this process
        void terminate();

        // Process over instance lifetime
        virtual void update(int dt);
    };
}

#endif