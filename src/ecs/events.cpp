#include "events.h"

namespace Dynamo {
    void EventManager::subscribe(System *system, std::string event_id) {
        std::vector<System *> &list = subscribers_[event_id];
        if(list.count(system)) {
            return;
        }
        list.push_back(system);
    }

    void EventManager::unsubscribe(System *system, std::string event_id) {
        std::vector<System *> &list = subscribers_[event_id];
        list.erase(
            std::remove(list.begin(), list.end(), system), 
            list.end()
        );
    }

    void EventManager::create_event(Event event) {
        events_.push_back(event);
    }

    void EventManager::update(unsigned dt) {
        Event latest = events_.push();
        for(auto &subscriber : subscribers_[latest.id]) {
            subscriber->handle_event(latest, dt);
        }
        events_.pop_front();
    }
}