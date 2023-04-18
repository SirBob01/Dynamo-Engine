#pragma once

#include <array>
#include <string>

#include <sndfile.hh>

#include "../../Log/Log.hpp"
#include "../../Sound/Resample.hpp"
#include "../../Sound/Sound.hpp"
#include "../../Types.hpp"

namespace Dynamo {
    /**
     * @brief Sound loader.
     *
     */
    class SoundLoader {
      public:
        /**
         * @brief Load a sound from a file.
         *
         * @param filepath
         * @return Sound::Sound
         */
        static Sound::Sound load(const std::string filepath);
    };
} // namespace Dynamo