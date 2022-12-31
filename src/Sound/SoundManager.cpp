#include "SoundManager.hpp"

namespace Dynamo::Sound {
    Asset<Sound> SoundManager::load_file(const std::string filename) {
        SndfileHandle file(filename.c_str());
        if (file.error()) {
            Log::error("Could not load sound file `{}`", filename);
        }

        unsigned frames = file.frames();
        unsigned channels = file.channels();
        unsigned sample_rate = file.samplerate();

        // Read the PCM data
        WaveForm interleaved(frames * channels);
        file.readf(interleaved.data(), interleaved.size());

        // De-interleave the data into the final waveform buffer
        WaveForm waveform(frames * channels);
        for (unsigned f = 0; f < frames; f++) {
            for (unsigned c = 0; c < channels; c++) {
                waveform[c * frames + f] = interleaved[f * channels + c];
            }
        }
        return allocate(waveform, channels, sample_rate);
    }

    Asset<Sound> SoundManager::load_raw(const WaveForm waveform,
                                        const unsigned channels,
                                        const float sample_rate) {
        return allocate(waveform, channels, sample_rate);
    }
} // namespace Dynamo::Sound