#ifndef DYNAMO_EVENTS_H_
#define DYNAMO_EVENTS_H_

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

namespace Dynamo {
    // Forward declarations
    typedef unsigned long EntityID;
    class System;

    // Event definition
    struct Event {
        std::string id;
        EntityID sender;
        EntityID receiver;
    };

    class EventManager {
        std::unordered_map<
            std::string, std::vector<System *>
        > subscribers_;

        std::queue<Event> events_;

    public:
        // Subscribe an system to an event
        void subscribe(System *system, std::string event_id);

        // Unsubscribe a system from an event (if it is already subscribed)
        void unsubscribe(System *system, std::string event_id);

        // Instantiate a new event
        void create_event(Event event);

        // Update all subscribers for any relevant events
        void update(unsigned dt);
    };
}

#endif