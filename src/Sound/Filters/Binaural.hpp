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
        HRTF _hrtf;
        std::array<Convolver, 2> _convolvers;

        Sound _impulse_response;
        Sound _remixed;
        Sound _output;

      public:
        /**
         * @brief Construct a new Binaural filter object.
         *
         */
        Binaural();

        Sound &apply(Sound &src,
                     const unsigned offset,
                     const unsigned length,
                     const Material &material,
                     const Listener &listener) override;
    };
} // namespace Dynamo::Sound