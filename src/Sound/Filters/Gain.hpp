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

        /**
         * @brief Construct a new Gain object.
         *
         */
        Gain();

        void transform(FilterContext context) override;
    };
} // namespace Dynamo::Sound