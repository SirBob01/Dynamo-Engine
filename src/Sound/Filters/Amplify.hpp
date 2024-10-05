#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Change the volume of a signal.
     *
     */
    struct Amplify : public Filter {
        /**
         * @brief Signal multiplier.
         *
         */
        float gain = 1;

        void apply(const Buffer &src,
                   Buffer &dst,
                   const Source &source,
                   const Listener &listener) override;
    };
} // namespace Dynamo::Sound