#pragma once

#include <array>

#include "../../Types.hpp"
#include "../Convolver.hpp"
#include "../HRTF.hpp"
#include "./Filter.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Binaural surround sound spatial filter implementation using HRTF
     *
     */
    class Binaural : public DynamicFilter {
        /**
         * @brief Impulse response container
         *
         */
        Sound _impulse_response;

        /**
         * @brief Signal convolvers
         *
         */
        std::array<Convolver, 2> _convolvers;

        /**
         * @brief Output buffer
         *
         */
        Sound _output;

      public:
        /**
         * @brief Construct a new Binaural filter object
         *
         */
        Binaural();

        Sound &apply(Sound &src,
                     const u32 src_offset,
                     const u32 length,
                     const DynamicMaterial &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound