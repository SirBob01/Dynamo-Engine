#pragma once

#include <array>

#include "../../Types.hpp"
#include "../Convolver.hpp"
#include "../EffectNode.hpp"
#include "../HRTF.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Binaural surround sound spatial filter implementation using HRTF.
     *
     */
    class Binaural : public EffectNode {
        std::reference_wrapper<HRTF> _hrtf;
        std::array<Convolver, 2> _convolvers;

        Sound _impulse_response;
        Sound _output;

        /**
         * @brief Find the closest listener to the sound source.
         *
         * @param listeners
         * @return ListenerProperties&
         */
        ListenerProperties &find_listener(const ListenerSet &listeners);

      public:
        /**
         * @brief Position of the sound source.
         *
         */
        Vec3 position;

        /**
         * @brief Construct a new Binaural object.
         *
         * @param hrtf HRTF interpolator
         */
        Binaural(HRTF &hrtf);

        Sound &transform(Sound &src,
                         u32 offset,
                         u32 length,
                         ListenerSet &listeners) override;
    };
} // namespace Dynamo::Sound