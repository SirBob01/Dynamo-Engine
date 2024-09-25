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
        unsigned size();

        /**
         * @brief Create a new listener
         *
         * @return Listener
         */
        Listener create();

        /**
         * @brief Destroy a listener, if it exists
         *
         * This will invalidate all existing references provided by
         * `get_properties()`
         *
         * @param listener
         */
        void destroy(Listener listener);

        /**
         * @brief Get the properties of a listener
         *
         * This reference is designed to be temporary and should not be cached
         *
         * @param listener
         * @return ListenerProperties&
         */
        ListenerProperties &get_properties(Listener listener);

        /**
         * @brief Clear all listeners
         *
         * This will invalidate all existing references provided by
         * `get_properties()`
         *
         */
        void clear();

        /**
         * @brief Random access operator to the listener pool
         *
         * @param index
         * @return ListenerProperties&
         */
        ListenerProperties &operator[](unsigned index);

        /**
         * @brief Find the closest listener to a given position.
         *
         * @param position
         * @return ListenerProperties&
         */
        ListenerProperties &find_closest(Vec3 position);
    };
} // namespace Dynamo::Sound