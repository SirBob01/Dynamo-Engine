#pragma once

#include <functional>

#include <Math/Quaternion.hpp>
#include <Math/Vec3.hpp>
#include <Utils/IdTracker.hpp>
#include <Utils/SparseSet.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Unique person listening to the sound scene.
     *
     */
    struct Listener {
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
     * @brief Listener Reference.
     *
     */
    using ListenerRef = std::reference_wrapper<Listener>;
} // namespace Dynamo::Sound