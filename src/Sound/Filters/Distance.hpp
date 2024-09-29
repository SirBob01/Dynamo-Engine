#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Attenuate volume as a sound moves away from the listener.
     *
     */
    class Distance : public Filter {
        float _inner_radius = 0;
        float _outer_radius = 1000;

        /**
         * @brief Linear attenuation function
         *
         * @param distance
         * @return float
         */
        float linear(float distance);

      public:
        /**
         * @brief Set the inner radius.
         *
         * @param radius
         */
        void set_inner_radius(float radius);

        /**
         * @brief Set the outer radius.
         *
         * @param radius
         */
        void set_outer_radius(float radius);

        void apply(const Sound &src,
                   Sound &dst,
                   const Material &material,
                   const Listener &listener) override;
    };
} // namespace Dynamo::Sound