#include "jukebox.h"

namespace Dynamo {
	Jukebox::Jukebox() {
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048);
		
		ambient_channel_ = 1;

		master_volume_ = 1.0;
		volume_conversion_ = 128.0;
	}

	Jukebox::~Jukebox() {
		skip_music(0);
		skip_ambient(0);

		clear_music_stream();
		clear_ambient_stream();

		for(auto &item : music_map_) {
			Mix_FreeMusic(item.second);
		}
		for(auto &item : chunk_map_) {
			Mix_FreeChunk(item.second);
		}

		music_map_.clear();
		chunk_map_.clear();
		Mix_CloseAudio();
	}

	float Jukebox::get_master_volume() {
		return master_volume_;
	}

	float Jukebox::get_music_volume() {
		return Mix_VolumeMusic(-1) / volume_conversion_;
	}

	float Jukebox::get_sfx_volume() {
		return Mix_Volume(-1, -1) / volume_conversion_;
	}

	void Jukebox::set_master_volume(float volume) {
		master_volume_ = volume;
	}

	void Jukebox::set_music_volume(float volume) {
		Mix_VolumeMusic(master_volume_ * volume * volume_conversion_);
	}

	void Jukebox::set_sfx_volume(float volume) {
		Mix_Volume(-1, master_volume_ * volume * volume_conversion_);
	}

	void Jukebox::queue_music(std::string filename, int ms) {
		if(!music_map_.count(filename)) {
			music_map_[filename] = Mix_LoadMUS(filename.c_str());
		}
		music_stream_.push_back(std::make_pair(music_map_[filename], ms));
	}

	void Jukebox::stream_music() {
		if(!Mix_PlayingMusic() && !music_stream_.empty()) {
			std::pair<Mix_Music *, int> &current = music_stream_.front();
			Mix_FadeInMusic(current.first, -1, current.second);
			music_stream_.pop_front();
		}
	}

	void Jukebox::stream_ambient() {
		if(!Mix_Playing(ambient_channel_) && !ambient_stream_.empty()) {
			std::pair<Mix_Chunk *, int> &current = ambient_stream_.front();
			Mix_FadeInChannel(ambient_channel_, current.first, -1, current.second);
			ambient_stream_.pop_front();
		}
	}

	void Jukebox::pause_music() {
		Mix_PauseMusic();
	}

	void Jukebox::resume_music() {
		Mix_ResumeMusic();
	}

	void Jukebox::skip_music(int ms) {
		Mix_FadeOutMusic(ms);
	}

	void Jukebox::queue_ambient(std::string filename, int ms) {
		if(!chunk_map_.count(filename)) {
			chunk_map_[filename] = Mix_LoadWAV(filename.c_str());
		}
		ambient_stream_.push_back(std::make_pair(chunk_map_[filename], ms));
	}

	void Jukebox::pause_ambient() {
		Mix_Pause(ambient_channel_);
	}

	void Jukebox::resume_ambient() {
		Mix_Resume(ambient_channel_);
	}

	void Jukebox::skip_ambient(int ms) {
		Mix_FadeOutChannel(ambient_channel_, ms);
	}

	void Jukebox::play_sfx(std::string filename, int loops) {
		if(!chunk_map_.count(filename)) {
			chunk_map_[filename] = Mix_LoadWAV(filename.c_str());
		}
		Mix_PlayChannel(-1, chunk_map_[filename], loops);
	}

	void Jukebox::clear_music_stream() {
		music_stream_.clear();
	}

	void Jukebox::clear_ambient_stream() {
		ambient_stream_.clear();
	}
}