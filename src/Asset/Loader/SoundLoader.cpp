#include "SoundLoader.hpp"

namespace Dynamo {
    Sound::Sound SoundLoader::load(const std::string filepath) {
        SndfileHandle file(filepath.c_str());
        if (file.error()) {
            Log::error("Could not load sound file `{}`", filepath);
        }

        u32 frames = file.frames();
        u32 channels = file.channels();
        u32 sample_rate = file.samplerate();

        // Read the PCM data
        Sound::WaveForm interleaved(frames * channels);
        file.readf(interleaved.data(), interleaved.size());

        // De-interleave the data into the final waveform buffer
        Sound::WaveForm waveform(frames * channels);
        for (u32 f = 0; f < frames; f++) {
            for (u32 c = 0; c < channels; c++) {
                waveform[c * frames + f] = interleaved[f * channels + c];
            }
        }

        return Sound::Sound(waveform, channels, sample_rate);
    }
} // namespace Dynamo