#pragma once

#include "../../Types.hpp"
#include "../Filter.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Gain filter implementation.
     *
     */
    class Gain : public Filter {
      public:
        /**
         * @brief Gain multiplier
         *
         */
        f32 gain = 1.0;

        void transform(Sound &src,
                       u32 offset,
                       u32 length,
                       ListenerSet &listeners) override;
    };
} // namespace Dynamo::Sound