#include "Sound.hpp"

namespace Dynamo {
    Sound::Sound(std::string filename) {
        stb_vorbis *vb =
            stb_vorbis_open_filename(filename.c_str(), nullptr, nullptr);
        if (!vb) {
            Log::error("Couldn't load Ogg Vorbis: {}", filename);
        }
        stb_vorbis_info info = stb_vorbis_get_info(vb);
        channels = info.channels;

        // Decode the waveform
        float buffer[2048];
        stb_vorbis_seek_start(vb);
        int read = -1;
        while (read) {
            read = stb_vorbis_get_samples_float_interleaved(vb,
                                                            channels,
                                                            buffer,
                                                            2048);
            for (int i = 0; i < read * 2; i++) {
                waveform.emplace_back(buffer[i]);
            }
        }
        stb_vorbis_close(vb);
    }
} // namespace Dynamo