#pragma once

#include <sndfile.hh>

#include <Sound/Sound.hpp>

namespace Dynamo {
    /**
     * @brief Load a sound file.
     *
     * @param filepath
     * @return Sound::Sound
     */
    Sound::Sound load_sound(const std::string filepath);
} // namespace Dynamo