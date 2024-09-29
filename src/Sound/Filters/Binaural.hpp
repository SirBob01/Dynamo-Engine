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

      public:
        /**
         * @brief Construct a new Binaural filter object.
         *
         */
        Binaural();

        void apply(const Sound &src,
                   Sound &dst,
                   const Material &material,
                   const Listener &listener) override;
    };
} // namespace Dynamo::Sound