#pragma once

#include <functional>

#include <Math/Vec3.hpp>
#include <Sound/Sound.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Parameters that describe playback information for the Sound.
     *
     */
    struct Material {
        /**
         * @brief Start time offset in seconds.
         *
         */
        Seconds start = 0s;

        /**
         * @brief Duration time (from the start time offset) in seconds.
         *
         */
        Seconds duration = -1s;

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

        /**
         * @brief Callback for when the sound is finished playing.
         *
         */
        std::function<void(Sound &)> on_finish = [](auto) {};
    };

    /**
     * @brief Material reference.
     *
     */
    using MaterialRef = std::reference_wrapper<Material>;
} // namespace Dynamo::Sound