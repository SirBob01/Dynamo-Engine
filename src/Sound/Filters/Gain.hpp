#pragma once

#include "../../Types.hpp"
#include "../EffectNode.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Gain filter implementation.
     *
     */
    class Gain : public EffectNode {
        Sound _output;

      public:
        /**
         * @brief Gain multiplier
         *
         */
        f32 gain = 1.0;

        Sound &transform(Sound &src,
                         u32 offset,
                         u32 length,
                         ListenerSet &listeners) override;
    };
} // namespace Dynamo::Sound