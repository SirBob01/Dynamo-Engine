#ifndef GAME_JUKEBOX
#define GAME_JUKEBOX

#include <SDL2/SDL_mixer.h>
#include <unordered_map>
#include <deque>
#include <utility>
#include <string>

class GameJukebox {
	std::deque<std::pair<Mix_Music *, int>> music_stream;
	std::deque<std::pair<Mix_Chunk *, int>> ambient_stream;

	std::unordered_map<std::string, Mix_Music *> music_map;
	std::unordered_map<std::string, Mix_Chunk *> chunk_map;
	
	int ambient_channel;

	float master_volume;
	float volume_conversion; // Conversion rate of SDL_Mixer volume
	
public:
	GameJukebox();
	~GameJukebox();

	float get_master_volume();
	float get_music_volume();
	float get_sfx_volume();

	// Volume needs to be a value between 0.0 and 1.0
	void set_master_volume(float volume);
	void set_music_volume(float volume);
	void set_sfx_volume(float volume);

	/* Stream functions (stream_music and stream_ambient) 
	are called in main game_loop */
	void stream_music();
	void stream_ambient();
	
	void clear_music_stream();
	void clear_ambient_stream();

	// Play functions
	void queue_music(std::string filename, int ms);
	void pause_music();
	void resume_music();
	void skip_music(int ms);

	void queue_ambient(std::string filename, int ms);
	void pause_ambient();
	void resume_ambient();
	void skip_ambient(int ms);

	void play_sfx(std::string filename, int loops);
};

#endif