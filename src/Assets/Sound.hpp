#pragma once

#include <sndfile.hh>

#include "../Sound/Sound.hpp"

namespace Dynamo {
    /**
     * @brief Load a sound file.
     *
     * @param filepath
     * @return Sound::Sound
     */
    Sound::Sound load(const std::string filepath);
} // namespace Dynamo