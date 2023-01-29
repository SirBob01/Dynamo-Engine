#pragma once

#include "../../Types.hpp"
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
                     const u32 src_offset,
                     const u32 length,
                     const DynamicMaterial &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound