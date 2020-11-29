#ifndef DYNAMO_JUKEBOX_H_
#define DYNAMO_JUKEBOX_H_
#define STB_VORBIS_HEADER_ONLY

#include <SDL2/SDL.h>

#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <memory>
#include <cmath>

#include "../assets/stb_vorbis.c"
#include "../util/ringbuffer.h"
#include "../util/vector.h"
#include "../util/util.h"
#include "../log/error.h"

namespace Dynamo {
    // A sound object is an array of PCM values (shorts)
    using Sound = std::vector<short>;

    // A unique integer handle for a sound stream
    using SoundStream = unsigned int;

    // Audio engine
    class Jukebox {
        // Represent an audio file
        class AudioFile {
            stb_vorbis *vb;

        public:
            AudioFile(std::string filename);
            ~AudioFile();

            stb_vorbis *get_vorbis();
        };

        // Sound data passed into mixer
        struct Chunk {
            Sound *sound;
            int written;
            float volume;
            Vec2D position;
        };

        // A queued stream unit
        struct StreamQueued {
            std::shared_ptr<AudioFile> file;

            // Time measures in seconds
            double duration;
            double fadein;
            double fadeout;
            
            int loops;
        };

        // A sound stream queue
        struct StreamLine {
            Sound track;
            std::queue<StreamQueued> queue;
            
            float volume;
            float max_volume;
            Vec2D position;

            double time; // Time position on current track
            int loop_counter;

            bool playing;

            StreamLine();
        };

        Sound base_;
        RingBuffer record_;

        SDL_AudioSpec output_spec_;
        SDL_AudioSpec input_spec_;

        // Playback and recording devices
        SDL_AudioDeviceID output_;
        SDL_AudioDeviceID input_;

        std::unordered_map<std::string, std::shared_ptr<AudioFile>> bank_;
        std::vector<StreamLine> streams_;
        std::vector<Chunk> chunks_;

        float master_volume_;

        // Maximum distance from audio source in pixels
        float max_distance_;

        // SDL_Audio playback function
        static void play_callback(void *data, uint8_t *stream, int length);
        
        // SDL_Audio record function
        static void record_callback(void *data, uint8_t *stream, int length);

        // Load an audio file
        void load_file(std::string filename);

        // Mix src into dst and clip the amplitude
        void mix_raw(short *dst, short *src, int length, 
                     float volume, Vec2D position);

        // Mix a chunk of sound onto the main track
        void mix_chunk(Chunk &chunk, int current_size);

        // Mix a streaming track onto main track
        void mix_stream(StreamLine &stream, int current_size);

        // Sanity check for stream IDs
        void check_stream_validity(SoundStream stream);

    public:
        Jukebox();
        ~Jukebox();

        // Check if the audio device is playing or paused
        bool is_playing();

        // Check if the device is currently receiving input
        bool is_recording();

        // Get the master volume
        float get_volume();

        // Get the maximum distance for positional audio
        float get_max_distance();

        // Set the master volume
        void set_volume(float volume);

        // Set the maximum distance for positional audio
        void set_max_distance(float distance);

        // Generate a streaming track
        // Return the index to the stream
        SoundStream generate_stream();

        // Is the stream playing
        bool is_stream_playing(SoundStream stream);

        // Is there anything on the stream's queue
        bool is_stream_empty(SoundStream stream);

        // Check if stream is transitioning between files
        bool is_stream_transition(SoundStream stream);

        // Get the volume of a stream
        float get_stream_volume(SoundStream stream);

        // Set the stream volume
        void set_stream_volume(SoundStream stream, float volume);

        // Set the position of the stream relative to the listener
        void set_stream_position(SoundStream stream, Vec2D position);

        // Queue a new audio into a streaming track
        // Allows fading in and out timing in seconds
        // Allows looping tracks (-1 for infinite loops)
        // Infinite loops can only terminate with clear_stream()
        void queue_stream(std::string filename, SoundStream stream, 
                          double fadein=0, double fadeout=0, 
                          int loops=1);

        // Play a stream
        void play_stream(SoundStream stream);

        // Pause a stream
        void pause_stream(SoundStream stream);

        // Skip current playing track on stream
        void skip_stream(SoundStream stream, double fadeout=0);

        // Clear all tracks on the stream
        void clear_stream(SoundStream stream);

        // Load a sound bite from a file
        Sound load_sound(std::string filename);

        // Play a sound bite
        void play_sound(Sound &sound, float volume=1.0, 
                        Vec2D position={0, 0});

        // Copy contents of internal record buffer to target buffer
        void stream_recorded(Sound &target);

        // Resume all audio playback
        void play();

        // Pause all audio playback
        void pause();

        // Resume audio recording
        void start_record();

        // Pause audio recording
        void pause_record();

        // Update the audio engine
        void update(unsigned dt);

        // Clear all active chunks and streams
        void clear();
    };
}

#endif