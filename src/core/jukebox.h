#ifndef DYNAMO_JUKEBOX_H_
#define DYNAMO_JUKEBOX_H_

#include <SDL2/SDL.h>

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

#include "../log/error.h"
#include "../sound/oggvorbis/vorbis/codec.h"
#include "../sound/oggvorbis/vorbis/vorbisfile.h"

namespace Dynamo {
    // Represent an audio file
    class AudioFile {
        FILE *file;         // Byte file
        OggVorbis_File vb;  // Encoded Ogg file

    public:
        AudioFile(std::string filename);
        ~AudioFile();

        OggVorbis_File *get_encoded();
    };

    // Raw base sound data
    struct Sound {
        char *samples;
        uint32_t length;
    };

    // Holds a buffer of mixed samples sent to device
    struct Track {
        char *buffer;
        int length;
        int written;

        Track();
        ~Track();
    };

    // An instance of playing sound
    struct Chunk {
        Sound sound;
        float volume;
    };

    // A sound stream
    struct Stream {
        Track track;
        std::queue<AudioFile *> queue;
        
        float volume;
        int fadeout;
        int fadein;
        bool playing;

        Stream();
    };

    // Main audio engine
    class Jukebox {
        Track base_;

        SDL_AudioSpec device_spec_;
        SDL_AudioDeviceID device_;

        std::unordered_map<std::string, AudioFile *> bank_;
        std::vector<Stream *> streams_;
        std::vector<Chunk> chunks_;

        float master_volume_;

        // SDL_Audio callback function
        static void callback(void *data, uint8_t *stream, int length);

        // Load an audio file
        AudioFile *load_file(std::string filename);

        // Mix src into dst and clip the amplitude
        void mix_raw(char *dst, char *src, int length, float volume);

        // Mix a chunk of sound onto the main track
        void mix_chunk(Chunk *chunk, int *max_copy);

        // Mix a streaming track onto main track
        void mix_stream(Stream *stream, int *max_copy);

        // Sanity check for stream IDs
        void check_stream_validity(int stream);

    public:
        /**
         * TODO:
         * Implement fading effects
         */
        Jukebox();
        ~Jukebox();

        // Check if the audio device is playing or paused
        bool is_playing();

        // Get the master volume
        float get_volume();

        // Set the master volume
        void set_volume(float volume);

        // Generate a streaming track
        // Return the index to the stream
        int generate_stream();

        // Get the volume of a stream
        float get_stream_volume(int stream);

        // Set the stream volume
        void set_stream_volume(int stream, float volume);

        // Queue a new audio into a streaming track
        void queue_stream(std::string filename, int stream, 
                          int fadein_ms=0, int fadeout_ms=0);

        // Play a stream
        void play_stream(int stream, int fadein_ms=0);

        // Pause a stream
        void pause_stream(int stream, int fadeout_ms=0);

        // Skip current playing track on stream
        void skip_stream(int stream, int fadeout_ms=0);

        // Clear all tracks on the stream
        void clear_stream(int stream);

        // Load a sound bite
        // Do not lose the reference to this pointer
        // -- Jukebox will not destroy it for you
        Sound *load_sound(std::string filename);

        // Destroy a sound bite
        void destroy_sound(Sound *sound);

        // Play a sound bite
        void play_sound(Sound *sound, float volume=1.0);

        // Resume all audio playback
        void play();

        // Pause all audio playback
        void pause();

        // Update tick
        void update();

        // Clear all active chunks and streams
        void clear();
    };

}

#endif