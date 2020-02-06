#ifndef DYNAMO_JUKEBOX_H_
#define DYNAMO_JUKEBOX_H_

#include <SDL2/SDL.h>

#include <unordered_map>
#include <vector>
#include <string>

#include "../sound/oggvorbis/vorbis/codec.h"
#include "../sound/oggvorbis/vorbis/vorbisfile.h"

namespace Dynamo {
    struct Sound {
        char *samples;
        uint32_t length;
    };

    struct Chunk {
        Sound sound;
        float volume;
    };

    struct Track {
        char *buffer;
        int length;
        int written;

        Track();
        ~Track();
    };

    class AudioFile {
        FILE *file;         // Byte file
        OggVorbis_File vb;  // Encoded Ogg file
        bool valid;

    public:
        AudioFile(std::string filename);
        ~AudioFile();

        bool is_valid();

        OggVorbis_File *get_encoded();
    };


    class Jukebox {
        Track base_;

        SDL_AudioSpec device_spec_;
        SDL_AudioDeviceID device_;

        std::unordered_map<std::string, AudioFile *> bank_;
        std::vector<Chunk> chunks_;

        float master_volume_;

        static void callback(void *data, uint8_t *stream, int length);

        // Load an audio file
        AudioFile *load_file(std::string filename);

        // Mix src into dst and clip the amplitude
        void mix_raw(char *dst, char *src, int length, float volume);

    public:
        Jukebox();
        ~Jukebox();

        bool is_playing();

        float get_volume();

        Sound *load_sound(std::string filename);

        void destroy_sound(Sound *sound);

        void play_sound(Sound *sound, float volume=1.0);

        void set_volume(float volume);

        void play();

        void pause();

        void mix_chunk(Chunk *chunk, int *max_copy);

        void update();
    };

}

#endif