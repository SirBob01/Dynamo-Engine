#pragma once

#include <functional>

#include "../Math/Vec3.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Parameters that describe spatial information about the sound
     *
     */
    struct Material {
        /**
         * @brief Volume to be played relative to overall gain.
         *
         */
        float volume = 1.0;

        /**
         * @brief Start time in seconds.
         *
         */
        float start_seconds = 0.0;

        /**
         * @brief Position of the sound in 3D space.
         *
         */
        Vec3 position;

        /**
         * @brief Velocity of the sound in 3D space.
         *
         */
        Vec3 velocity;
    };

    /**
     * @brief Material reference.
     *
     */
    using MaterialRef = std::reference_wrapper<Material>;
} // namespace Dynamo::Sound