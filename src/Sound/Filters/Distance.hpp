#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Attenuate volume as a sound moves away from the listener.
     *
     */
    struct Distance : public Filter {
        /**
         * @brief Inner radius of the attenuation sphere.
         *
         */
        float inner_radius = 0;

        /**
         * @brief Outer radius of the attenuation sphere.
         *
         */
        float outer_radius = 1000;

        /**
         * @brief Linear attenuation function
         *
         * @param distance
         * @return float
         */
        float linear(float distance);

        void apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) override;
    };
} // namespace Dynamo::Sound