#pragma once

#include <array>

#include <Sound/DSP/Convolver.hpp>
#include <Sound/DSP/HRTF.hpp>
#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Binaural spatial sound implementation using HRTF.
     *
     */
    class Binaural : public Filter {
        std::reference_wrapper<HRTF> _hrtf;
        std::array<Convolver, 2> _convolvers;

        Sound _impulse_response;
        Sound _remixed;
        Sound _output;

      public:
        /**
         * @brief Construct a new Binaural filter object.
         *
         * @param hrtf HRTF interpolator
         */
        Binaural(HRTF &hrtf);

        Sound &apply(Sound &src,
                     const unsigned offset,
                     const unsigned length,
                     const Material &material,
                     const ListenerProperties &listener) override;
    };
} // namespace Dynamo::Sound