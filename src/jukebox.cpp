#include "jukebox.h"

GameJukebox::GameJukebox() {
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
	
	ambient_channel = 1;

	master_volume = 1.0;
	volume_conversion = 128.0;
}

GameJukebox::~GameJukebox() {
	skip_music(0);
	skip_ambient(0);

	clear_music_stream();
	clear_ambient_stream();

	for(auto &item : music_map) {
		Mix_FreeMusic(item.second);
	}
	for(auto &item : chunk_map) {
		Mix_FreeChunk(item.second);
	}

	music_map.clear();
	chunk_map.clear();
	Mix_CloseAudio();
}

float GameJukebox::get_master_volume() {
	return master_volume;
}

float GameJukebox::get_music_volume() {
	return Mix_VolumeMusic(-1) / volume_conversion;
}

float GameJukebox::get_sfx_volume() {
	return Mix_Volume(-1, -1) / volume_conversion;
}

void GameJukebox::set_master_volume(float volume) {
	master_volume = volume;
}

void GameJukebox::set_music_volume(float volume) {
	Mix_VolumeMusic(master_volume * volume * volume_conversion);
}

void GameJukebox::set_sfx_volume(float volume) {
	Mix_Volume(-1, master_volume * volume * volume_conversion);
}

void GameJukebox::queue_music(std::string filename, int ms) {
	if(!music_map.count(filename)) {
		music_map[filename] = Mix_LoadMUS(filename.c_str());
	}
	music_stream.push_back(std::make_pair(music_map[filename], ms));
}

void GameJukebox::stream_music() {
	if(!Mix_PlayingMusic() && !music_stream.empty()) {
		std::pair<Mix_Music *, int> &current = music_stream.front();
		Mix_FadeInMusic(current.first, -1, current.second);
		music_stream.pop_front();
	}
}

void GameJukebox::stream_ambient() {
	if(!Mix_Playing(ambient_channel) && !ambient_stream.empty()) {
		std::pair<Mix_Chunk *, int> &current = ambient_stream.front();
		Mix_FadeInChannel(ambient_channel, current.first, -1, current.second);
		ambient_stream.pop_front();
	}
}

void GameJukebox::pause_music() {
	Mix_PauseMusic();
}

void GameJukebox::resume_music() {
	Mix_ResumeMusic();
}

void GameJukebox::skip_music(int ms) {
	Mix_FadeOutMusic(ms);
}

void GameJukebox::queue_ambient(std::string filename, int ms) {
	if(!chunk_map.count(filename)) {
		chunk_map[filename] = Mix_LoadWAV(filename.c_str());
	}
	ambient_stream.push_back(std::make_pair(chunk_map[filename], ms));
}

void GameJukebox::pause_ambient() {
	Mix_Pause(ambient_channel);
}

void GameJukebox::resume_ambient() {
	Mix_Resume(ambient_channel);
}

void GameJukebox::skip_ambient(int ms) {
	Mix_FadeOutChannel(ambient_channel, ms);
}

void GameJukebox::play_sfx(std::string filename, int loops) {
	if(!chunk_map.count(filename)) {
		chunk_map[filename] = Mix_LoadWAV(filename.c_str());
	}
	Mix_PlayChannel(-1, chunk_map[filename], loops);
}

void GameJukebox::clear_music_stream() {
	music_stream.clear();
}

void GameJukebox::clear_ambient_stream() {
	ambient_stream.clear();
}
