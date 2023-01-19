#include "SoundCache.hpp"

namespace Dynamo::Sound {
    SoundCache::SoundCache(const std::string asset_directory) :
        AssetCache<Sound>(asset_directory) {}

    Sound *SoundCache::load(const std::string filepath) {
        SndfileHandle file(filepath.c_str());
        if (file.error()) {
            Log::error("Could not load sound file `{}`", filepath);
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
        return new Sound(waveform, channels, sample_rate);
    }
} // namespace Dynamo::Sound