#pragma once

#include "./Filter.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Stereo panner filter implementation
     *
     */
    class Stereo : public DynamicFilter {
        /**
         * @brief Output buffer
         *
         */
        Sound _output;

      public:
        /**
         * @brief Construct a new Stereo filter object
         *
         */
        Stereo();

        Sound &apply(Sound &src,
                     const unsigned src_offset,
                     const unsigned length,
                     const DynamicMaterial &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound