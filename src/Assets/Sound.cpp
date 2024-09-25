#include <Assets/Sound.hpp>
#include <Utils/Log.hpp>

namespace Dynamo {
    Sound::Sound load(const std::string filepath) {
        SndfileHandle file(filepath.c_str(), SFM_READ, 0, 1, 0);
        if (file.error()) {
            Log::error("Could not load sound file `{}`: {}",
                       filepath,
                       file.strError());
        }

        unsigned frames = file.frames();
        unsigned channels = file.channels();
        unsigned sample_rate = file.samplerate();

        // Read the PCM data
        Sound::WaveForm interleaved(frames * channels);
        file.readf(interleaved.data(), interleaved.size());

        // De-interleave the data into the final waveform buffer
        Sound::WaveForm waveform(frames * channels);
        for (unsigned f = 0; f < frames; f++) {
            for (unsigned c = 0; c < channels; c++) {
                waveform[c * frames + f] = interleaved[f * channels + c];
            }
        }

        return Sound::Sound(waveform, channels, sample_rate);
    }
} // namespace Dynamo