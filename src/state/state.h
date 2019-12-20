#ifndef DYNAMO_STATE_H_
#define DYNAMO_STATE_H_

namespace Dynamo {
    class State {
    protected:
        State *parent_;
        State *next_;

        bool active_;

    public:
        State();
        virtual ~State();

        // Check if the state is active
        bool is_active();

        // Get previous state
        State *get_parent();

        // Get the next state
        State *get_next();

        // Set the parent state
        void set_parent(State *state);

        // Set the next state
        void set_next(State *state);

        // Terminate this process
        void terminate();

        // Destroy this state's ancestors
        void destroy_ancestors();

        // Process on entry
        virtual void on_entry();

        // Process on exit
        virtual void on_exit();

        // Process over instance lifetime
        virtual void update(unsigned dt);
    };
}

#endif