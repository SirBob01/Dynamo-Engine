#pragma once

#include "../../Math/Quaternion.hpp"
#include "../../Math/Vec3.hpp"
#include "../../Types.hpp"
#include "../EffectNode.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Stereo panner filter implementation.
     *
     */
    class Stereo : public EffectNode {
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

        Sound &process(Sound &src,
                       u32 offset,
                       u32 length,
                       ListenerSet &listeners) override;
    };
} // namespace Dynamo::Sound