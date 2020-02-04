#ifndef DYNAMO_JUKEBOX_H_
#define DYNAMO_JUKEBOX_H_

#include <SDL2/SDL_mixer.h>

#include <unordered_map>
#include <deque>
#include <utility>
#include <string>

namespace Dynamo {
    /** TODO: Jukebox Engine Specs
     *
     * Two types of sound
     *      Bite (Decoded once, stored in memory)
     *      Stream (Decoded on request from audio device)
     * Objects:
     *      Sound (Stores the file and encoded data)
     *      Jukebox (Main engine class)
     * Methods:
     *      int get_stream()
     *      int generate_stream()
     *
     *      Bite *load_bite(std::string filename)
     *      void play_bite(Bite *load_bite, float volume)
     *      
     *      void stream(std::string filename, float volume, int stream_id)
     *      void play_stream(int track_id)
     *      void pause_stream(int track_id)
     *      void stop_stream(int track_id)
     *
     *      void set_volume(float volume)
     *
     *      void stop()
     *      void play()
     *
     *      void update()
     */
    class Jukebox {
        // Unfortunately, SDL_Mixer does not allow multiple
        // concurrent music streams for ambient noise
        std::deque<std::pair<Mix_Music *, int>> music_stream_;
        std::deque<std::pair<Mix_Chunk *, int>> ambient_stream_;

        std::unordered_map<std::string, Mix_Music *> music_map_;
        std::unordered_map<std::string, Mix_Chunk *> chunk_map_;
        
        int ambient_channel_;

        float master_vol_;
        float vol_convert_; // Conversion rate of SDL_Mixer volume
        
    public:
        Jukebox();
        ~Jukebox();

        // Get current volume
        float get_master_volume();
        float get_music_volume();
        float get_sfx_volume();

        // Set volume to a value between 0.0 and 1.0
        void set_master_volume(float vol);
        void set_music_volume(float vol);
        void set_sfx_volume(float vol);

        // Stream audio (called in main engine game-loop)
        void stream_music();
        void stream_ambient();
        
        // Clear the audio streams
        void clear_music_stream();
        void clear_ambient_stream();

        // Music handling
        void queue_music(std::string filename, int ms);
        void pause_music();
        void resume_music();
        void skip_music(int ms);

        // Ambient noise handling
        void queue_ambient(std::string filename, int ms);
        void pause_ambient();
        void resume_ambient();
        void skip_ambient(int ms);

        // SFx handling
        void play_sfx(std::string filename, int loops);
    };
}

#endif