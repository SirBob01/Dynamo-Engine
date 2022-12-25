#include "SoundManager.hpp"

namespace Dynamo {
    Asset<Sound> SoundManager::load_ogg(const std::string filename) {
        stb_vorbis *vb =
            stb_vorbis_open_filename(filename.c_str(), nullptr, nullptr);
        if (!vb) {
            Log::error("Couldn't load Ogg Vorbis: {}", filename);
        }
        stb_vorbis_info info = stb_vorbis_get_info(vb);

        unsigned frames = stb_vorbis_stream_length_in_samples(vb);
        unsigned channels = info.channels;
        double sample_rate = info.sample_rate;

        // Decode the waveform
        std::vector<WaveSample> waveform(frames * channels);
        std::array<short, 1024> read_buffer;
        stb_vorbis_seek_start(vb);
        unsigned curr_frame = 0;
        while (true) {
            unsigned read =
                stb_vorbis_get_samples_short_interleaved(vb,
                                                         channels,
                                                         read_buffer.data(),
                                                         read_buffer.size());
            if (read == 0) {
                break;
            }
            for (unsigned c = 0; c < channels; c++) {
                for (unsigned r = 0; r < read; r++) {
                    WaveSample *offset = waveform.data() + (c * frames);
                    offset[r + curr_frame] = read_buffer[c + info.channels * r];
                }
            }
            curr_frame += read;
        }
        stb_vorbis_close(vb);

        return allocate(waveform, channels, sample_rate);
    }

    Asset<Sound> SoundManager::load_raw(const WaveForm waveform,
                                        const unsigned channels,
                                        const double sample_rate) {
        return allocate(waveform, channels, sample_rate);
    }
} // namespace Dynamo