#include <limits>

#include "./Listener.hpp"

namespace Dynamo::Sound {
    unsigned ListenerSet::size() { return _properties.size(); }

    Listener ListenerSet::create() {
        Listener listener = _id_tracker.generate();
        _properties.insert(listener);
        return listener;
    }

    void ListenerSet::destroy(Listener listener) {
        _properties.remove(listener);
        _id_tracker.discard(listener);
    }

    ListenerProperties &ListenerSet::get_properties(Listener listener) {
        return _properties.get(listener);
    }

    void ListenerSet::clear() {
        _properties.clear();
        _id_tracker.clear();
    }

    ListenerProperties &ListenerSet::operator[](unsigned index) {
        return _properties.at(index);
    }

    ListenerProperties &ListenerSet::find_closest(Vec3 position) {
        unsigned closest_index = 0;

        float closest_distance = std::numeric_limits<float>::max();
        for (unsigned i = 0; i < _properties.size(); i++) {
            ListenerProperties &listener = _properties.at(i);
            float distance = (listener.position - position).length_squared();
            if (distance < closest_distance) {
                closest_index = i;
                closest_distance = distance;
            }
        }
        return _properties.at(closest_index);
    }
} // namespace Dynamo::Sound