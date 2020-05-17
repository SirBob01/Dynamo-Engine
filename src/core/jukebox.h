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

#include "../util/ringbuffer.h"
#include "../util/util.h"
#include "../log/error.h"

namespace Dynamo {
    // A sound object is a byte array of PCM values
    using Sound = std::vector<char>;

    // A unique integer handle for a sound stream
    using SoundStream = unsigned int;

    // Audio engine
    class Jukebox {
        // Represent an audio file
        class AudioFile {
            FILE *file;         // Byte file
            OggVorbis_File vb;  // Encoded Ogg file

        public:
            AudioFile(std::string filename);
            ~AudioFile();

            OggVorbis_File *get_encoded();
        };

        // Sound data passed into mixer
        struct Chunk {
            Sound *sound;
            int written;
            float volume;
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

        // SDL_Audio playback function
        static void play_callback(void *data, uint8_t *stream, int length);
        
        // SDL_Audio record function
        static void record_callback(void *data, uint8_t *stream, int length);

        // Load an audio file
        void load_file(std::string filename);

        // Mix src into dst and clip the amplitude
        void mix_raw(char *dst, char *src, int length, float volume);

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

        // Set the master volume
        void set_volume(float volume);

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
        void play_sound(Sound &sound, float volume=1.0);

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