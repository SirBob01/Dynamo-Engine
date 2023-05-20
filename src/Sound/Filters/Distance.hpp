#pragma once

#include "../../Types.hpp"
#include "../Filter.hpp"
#include "../Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Distance attenuation filter allows a sound to decrease in volume
     * as it moves further away from the listener.
     *
     */
    class Distance : public Filter {
        f32 _inner_radius;
        f32 _cutoff_radius;

        /**
         * @brief Linear attenuation function.
         *
         * @param distance
         * @return f32
         */
        f32 linear(f32 distance);

      public:
        /**
         * @brief Position of the sound source.
         *
         */
        Vec3 position;

        /**
         * @brief Construct a new Distance object.
         *
         * @param inner_radius  Minimum distance to start attenuation.
         * @param cutoff_radius Maximum distance to cutoff the sound.
         */
        Distance(f32 inner_radius, f32 cutoff_radius);

        void transform(FilterContext context) override;
    };
} // namespace Dynamo::Sound