#pragma once

#include <memory>

#include "../Asset/Asset.hpp"
#include "../Math/Vec3.hpp"
#include "./Sound.hpp"

namespace Dynamo {
    /**
     * @brief Forward declarations for the filter types
     *
     */
    class StaticFilter;
    class DynamicFilter;

    /**
     * @brief Parameters that describe the effects and processing of static
     * sounds during playback
     *
     */
    struct StaticSoundMaterial {
        /**
         * @brief Volume to be played relative to overall gain
         *
         */
        double volume = 1.0;

        /**
         * @brief Start time in seconds
         *
         */
        double start_seconds = 0.0;

        /**
         * @brief Filter pipeline
         *
         */
        std::vector<std::unique_ptr<StaticFilter>> filters;
    };

    /**
     * @brief Parameters that describe spatial information about the sound
     *
     */
    struct DynamicSoundMaterial {
        /**
         * @brief Volume to be played relative to overall gain
         *
         */
        double volume = 1.0;

        /**
         * @brief Start time in seconds
         *
         */
        double start_seconds = 0.0;

        /**
         * @brief Position of the sound in 3D space
         *
         */
        Vec3 position;

        /**
         * @brief Velocity of the sound in 3D space to simulate the Doppler
         * effect
         *
         */
        Vec3 velocity;

        /**
         * @brief Filter pipeline
         *
         */
        std::vector<std::unique_ptr<DynamicFilter>> filters;
    };
} // namespace Dynamo