#pragma once

#include "../../Math/Quaternion.hpp"
#include "../../Math/Vec3.hpp"
#include "../../Types.hpp"
#include "../Filter.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Stereo panner filter implementation.
     *
     */
    class Stereo : public Filter {
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
         * @brief Construct a new Stereo object.
         *
         */
        Stereo();

        void transform(FilterContext context) override;
    };
} // namespace Dynamo::Sound