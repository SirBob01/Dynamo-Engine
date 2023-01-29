#pragma once

#include "../../Types.hpp"
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
        f32 _inner_radius;

        /**
         * @brief Maximum distance to attenuate where the audio is cutoff
         *
         */
        f32 _cutoff_radius;

        /**
         * @brief Linear attenuation function
         *
         * @param distance
         * @return f32
         */
        f32 linear(f32 distance);

      public:
        /**
         * @brief Construct a new Attenuation filter object
         *
         * @param inner_radius  Minimum distance to start attenuation
         * @param cutoff_radius Maximum distance to cutoff the sound
         */
        Attenuation(f32 inner_radius, f32 cutoff_radius);

        Sound &apply(Sound &src,
                     const u32 src_offset,
                     const u32 length,
                     const DynamicMaterial &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound