#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Attenuate volume as a sound moves away from the listener.
     *
     */
    class Attenuation : public Filter {
        Sound _output;

        float _inner_radius;
        float _outer_radius;
        float _denominator;

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
         * @param inner_radius Minimum distance to start attenuation
         * @param outer_radius Maximum distance to cutoff the sound
         */
        Attenuation(float inner_radius, float outer_radius);

        Sound &apply(Sound &src,
                     const unsigned offset,
                     const unsigned length,
                     const Material &material,
                     const Listener &listener) override;
    };
} // namespace Dynamo::Sound