#ifndef DYNAMO_JUKEBOX_H_
#define DYNAMO_JUKEBOX_H_

#include <SDL2/SDL.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "clock.h"
#include "../util/ringbuffer.h"
#include "../util/util.h"
#include "../log/error.h"

namespace Dynamo {
    // A unique integer handle for a stream
    using StreamID = int;

    // Represent an audio file
    class AudioFile {
        FILE *file;         // Byte file
        OggVorbis_File vb;  // Encoded Ogg file

    public:
        AudioFile(std::string filename);
        ~AudioFile();

        OggVorbis_File *get_encoded();
    };

    // Holds raw PCM data for sound storage
    struct Sound {
        char *samples;
        int length;
        int write;

        Sound(int size);
        ~Sound();
    };

    // Temporary sound data passed into mixer
    // Do not use as raw PCM storage, use Sound instead
    struct Chunk {
        char *samples;
        int length;
        float volume;
    };

    // A queued stream unit
    struct StreamMeta {
        std::shared_ptr<AudioFile> file;

        // Time measures in seconds
        double duration;
        double fadein;
        double fadeout;
        
        int loops;
        unsigned start_time; // Millisecond marker
    };

    // A sound stream
    struct Stream {
        Sound track;
        std::queue<StreamMeta> queue;
        
        float volume;
        float max_volume;

        double time; // Time position on current track
        int loop_counter;

        bool playing;

        Stream();
    };

    // Audio engine
    class Jukebox {
        Sound base_;
        RingBuffer record_;

        SDL_AudioSpec output_spec_;
        SDL_AudioSpec input_spec_;

        // Playback and recording devices
        SDL_AudioDeviceID output_;
        SDL_AudioDeviceID input_;

        std::unordered_map<std::string, std::shared_ptr<AudioFile>> bank_;
        std::vector<std::shared_ptr<Stream>> streams_;
        std::vector<Chunk> chunks_;

        float master_volume_;

        Clock *clock_;

        // SDL_Audio playback function
        static void play_callback(void *data, uint8_t *stream, int length);
        
        // SDL_Audio record function
        static void record_callback(void *data, uint8_t *stream, int length);

        // Load an audio file
        std::shared_ptr<AudioFile> &load_file(std::string filename);

        // Mix src into dst and clip the amplitude
        void mix_raw(char *dst, char *src, int length, float volume);

        // Mix a chunk of sound onto the main track
        void mix_chunk(Chunk &chunk, int *max_copy);

        // Mix a streaming track onto main track
        void mix_stream(Stream &stream, int *max_copy);

        // Sanity check for stream IDs
        void check_stream_validity(StreamID stream);

    public:
        Jukebox(Clock *clock);
        ~Jukebox();

        // Check if the audio device is playing or paused
        bool is_playing();

        // Get the master volume
        float get_volume();

        // Set the master volume
        void set_volume(float volume);

        // Generate a streaming track
        // Return the index to the stream
        StreamID generate_stream();

        // Is the stream playing
        bool is_stream_playing(StreamID stream);

        // Is there anything on the stream's queue
        bool is_stream_empty(StreamID stream);

        // Check if stream is transitioning between files
        bool is_stream_transition(StreamID stream);

        // Get the volume of a stream
        float get_stream_volume(StreamID stream);

        // Set the stream volume
        void set_stream_volume(StreamID stream, float volume);

        // Queue a new audio into a streaming track
        // Allows fading in and out timing in seconds
        // Allows looping tracks (-1 for infinite loops)
        // Infinite loops can only terminate with clear_stream()
        void queue_stream(std::string filename, StreamID stream, 
                          double fadein=0, double fadeout=0, 
                          int loops=1);

        // Play a stream
        void play_stream(StreamID stream);

        // Pause a stream
        void pause_stream(StreamID stream);

        // Skip current playing track on stream
        void skip_stream(StreamID stream, double fadeout=0);

        // Clear all tracks on the stream
        void clear_stream(StreamID stream);

        // Load a sound bite
        // Do not lose the reference to this pointer
        // Jukebox will not destroy it for you
        Sound *load_sound(std::string filename);

        // Play a sound bite
        void play_sound(Sound *sound, float volume=1.0);

        // Copy contents of internal record buffer to target buffer
        void stream_recorded(Sound *target);

        // Resume all audio playback
        void play();

        // Pause all audio playback
        void pause();

        // Resume audio recording
        void start_record();

        // Pause audio recording
        void pause_record();

        // Update tick
        void update();

        // Clear all active chunks and streams
        void clear();
    };
}

#endif