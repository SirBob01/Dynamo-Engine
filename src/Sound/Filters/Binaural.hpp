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

        Buffer _impulse_response;
        Buffer _mono;

      public:
        void apply(const Buffer &src, Buffer &dst, const Source &source, const Listener &listener) override;
    };
} // namespace Dynamo::Sound