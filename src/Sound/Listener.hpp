#pragma once

#include "../Math/Quaternion.hpp"
#include "../Math/Vec3.hpp"
#include "../Utils/IdTracker.hpp"
#include "../Utils/SparseSet.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Unique listener in the Jukebox engine
     *
     */
    using Listener = Id;

    /**
     * @brief Properties of a listener used to simulate various audio effects
     *
     */
    struct ListenerProperties {
        /**
         * @brief Listener volume
         *
         */
        float volume = 1.0;

        /**
         * @brief Position of the listener in 3D space
         *
         */
        Vec3 position;

        /**
         * @brief Velocity of the listener in 3D space
         *
         */
        Vec3 velocity;

        /**
         * @brief Orientation of the listener
         *
         */
        Quaternion rotation;
    };

    /**
     * @brief Maintains the set of all listeners
     *
     */
    class ListenerSet {
        /**
         * @brief Listener Id tracker
         *
         */
        IdTracker _id_tracker;

        /**
         * @brief Sparse set of all listeners
         *
         */
        SparseSet<ListenerProperties> _properties;

      public:
        /**
         * @brief Get the number of listeners
         *
         * @return unsigned
         */
        inline unsigned size() { return _properties.size(); }

        /**
         * @brief Create a new listener
         *
         * @return Listener
         */
        inline Listener create() {
            Listener listener = _id_tracker.generate();
            _properties.insert(listener);
            return listener;
        }

        /**
         * @brief Destroy a listener, if it exists
         *
         * This will invalidate all existing references provided by
         * `get_properties()`
         *
         * @param listener
         */
        inline void destroy(Listener listener) {
            _properties.remove(listener);
            _id_tracker.discard(listener);
        }

        /**
         * @brief Get the properties of a listener
         *
         * This reference is designed to be temporary and should not be cached
         *
         * @param listener
         * @return ListenerProperties&
         */
        inline ListenerProperties &get_properties(Listener listener) {
            return _properties.get(listener);
        }

        /**
         * @brief Clear all listeners
         *
         * This will invalidate all existing references provided by
         * `get_properties()`
         *
         */
        inline void clear() {
            _properties.clear();
            _id_tracker.clear();
        }

        /**
         * @brief Random access operator to the listener pool
         *
         * @param index
         * @return ListenerProperties&
         */
        inline ListenerProperties &operator[](unsigned index) {
            return _properties.at(index);
        }

        /**
         * @brief Find the closest listener to a given position.
         *
         * @param position
         * @return ListenerProperties&
         */
        ListenerProperties &find_closest(Vec3 position) {
            unsigned closest_index = 0;
            for (unsigned i = 0; i < _properties.size(); i++) {
                Vec3 best = _properties.at(i).position;
                Vec3 curr = _properties.at(closest_index).position;

                float a = (best - position).length_squared();
                float b = (curr - position).length_squared();
                if (b < a) {
                    closest_index = i;
                }
            }
            return _properties.at(closest_index);
        }
    };
} // namespace Dynamo::Sound