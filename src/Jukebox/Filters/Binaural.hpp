#pragma once

#include <array>

#include "../HRTF.hpp"
#include "./Convolver.hpp"
#include "./Filter.hpp"

namespace Dynamo {
    /**
     * @brief Binaural surround sound spatial filter implementation using HRTF
     *
     */
    class Binaural : public DynamicFilter {
        /**
         * @brief Reference to the HRTF impulse response generator
         *
         */
        HRTF &_hrtf;

        /**
         * @brief Impulse response coefficients
         *
         */
        ChannelData<Complex> _coeffs;

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
         * @param hrtf
         */
        Binaural(HRTF &hrtf);

        Sound &apply(Sound &src,
                     const unsigned src_offset,
                     const unsigned length,
                     const DynamicSoundMaterial &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo