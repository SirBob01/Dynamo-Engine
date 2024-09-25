#pragma once

#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Stereo panner filter implementation
     *
     */
    class Stereo : public Filter {
        Sound _output;

      public:
        /**
         * @brief Construct a new Stereo filter object
         *
         */
        Stereo();

        Sound &apply(Sound &src,
                     const unsigned offset,
                     const unsigned length,
                     const Material &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound