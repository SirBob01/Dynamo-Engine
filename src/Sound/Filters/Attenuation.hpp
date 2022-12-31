#pragma once

#include "./Filter.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Distance attenuation filter allows a sound to decrease in volume
     * as it moves further away from the listener
     *
     */
    class Attenuation : public DynamicFilter {
        /**
         * @brief Output buffer
         *
         */
        Sound _output;

        /**
         * @brief Minimum distance to attenuate
         *
         */
        float _inner_radius;

        /**
         * @brief Maximum distance to attenuate where the audio is cutoff
         *
         */
        float _cutoff_radius;

        /**
         * @brief Linear attenuation function
         *
         * @param distance
         * @return float
         */
        float linear(float distance);

      public:
        /**
         * @brief Construct a new Attenuation filter object
         *
         * @param inner_radius  Minimum distance to start attenuation
         * @param cutoff_radius Maximum distance to cutoff the sound
         */
        Attenuation(float inner_radius, float cutoff_radius);

        Sound &apply(Sound &src,
                     const unsigned src_offset,
                     const unsigned length,
                     const DynamicMaterial &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound