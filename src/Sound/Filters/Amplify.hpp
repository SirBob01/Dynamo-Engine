#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Change the volume of a sound signal.
     *
     */
    class Amplify : public Filter {
        float _gain = 1;

      public:
        /**
         * @brief Set the gain multiplier.
         *
         * @param gain
         */
        void set_gain(float gain);

        void apply(const Sound &src,
                   Sound &dst,
                   const Material &material,
                   const Listener &listener) override;
    };
} // namespace Dynamo::Sound