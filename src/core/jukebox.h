#ifndef DYNAMO_JUKEBOX_H_
#define DYNAMO_JUKEBOX_H_

#include <SDL2/SDL.h>

#include <unordered_map>
#include <vector>
#include <string>

#include "../log/error.h"
#include "../sound/oggvorbis/vorbis/codec.h"
#include "../sound/oggvorbis/vorbis/vorbisfile.h"

namespace Dynamo {
    // Raw base sound data
    struct Sound {
        char *samples;
        uint32_t length;
    };

    // An instance of playing sound
    struct Chunk {
        Sound sound;
        float volume;
    };

    // Track buffer for layering streams
    struct Track {
        char *buffer;
        int length;
        int written;

        Track();
        ~Track();
    };

    // Represent an audio file
    class AudioFile {
        FILE *file;         // Byte file
        OggVorbis_File vb;  // Encoded Ogg file

    public:
        AudioFile(std::string filename);
        ~AudioFile();

        OggVorbis_File *get_encoded();
    };

    // Main audio engine
    class Jukebox {
        Track base_;

        SDL_AudioSpec device_spec_;
        SDL_AudioDeviceID device_;

        std::unordered_map<std::string, AudioFile *> bank_;
        std::vector<Chunk> chunks_;

        float master_volume_;

        // SDL_Audio callback function
        static void callback(void *data, uint8_t *stream, int length);

        // Load an audio file
        AudioFile *load_file(std::string filename);

        // Mix src into dst and clip the amplitude
        void mix_raw(char *dst, char *src, int length, float volume);

    public:
        Jukebox();
        ~Jukebox();

        // Check if the audio device is playing or paused
        bool is_playing();

        // Get the master volume
        float get_volume();

        // Load a sound bite
        Sound *load_sound(std::string filename);

        // Destroy a sound bite
        void destroy_sound(Sound *sound);

        // Play a sound bite
        void play_sound(Sound *sound, float volume=1.0);

        // Set the master volume
        void set_volume(float volume);

        // Resume all audio playback
        void play();

        // Pause all audio playback
        void pause();

        // Mix a chunk of sound onto the main track
        void mix_chunk(Chunk *chunk, int *max_copy);

        // Update tick
        void update();

        // Clear all active chunks
        void clear();
    };

}

#endif