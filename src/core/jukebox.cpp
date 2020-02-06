#include "jukebox.h"

namespace Dynamo {
    Track::Track() {
        length = 4096;
        buffer = new char[length];
        SDL_memset(buffer, 0, length);

        written = 0;
    }

    Track::~Track() {
        delete[] buffer;
    }

    AudioFile::AudioFile(std::string filename) {
        file = fopen(filename.c_str(), "rb");
        valid = ov_open_callbacks(
            file, &vb, NULL, 0, OV_CALLBACKS_NOCLOSE
        ) >= 0;
    }

    AudioFile::~AudioFile() {
        ov_clear(&vb);
        fclose(file);
    }

    bool AudioFile::is_valid() {
        return valid;
    }

    OggVorbis_File *AudioFile::get_encoded() {
        return &vb;
    }

    Jukebox::Jukebox() {
        SDL_memset(&device_spec_, 0, sizeof(SDL_AudioSpec));
        device_spec_.channels = 2;
        device_spec_.freq = 44100;
        device_spec_.format = AUDIO_S16LSB;
        device_spec_.samples = 1024;
        device_spec_.userdata = &base_;
        device_spec_.callback = callback;

        device_ = SDL_OpenAudioDevice(
            nullptr, 0, &device_spec_, nullptr, 0
        );
        play();
    }

    Jukebox::~Jukebox() {
        for(auto &pair : bank_) {
            delete pair.second;
        }
    }

    void Jukebox::callback(void *data, uint8_t *stream, int length) {
        Track *base = static_cast<Track *>(data);

        SDL_memset(stream+base->written, 0, length-base->written);
        SDL_memcpy(stream, base->buffer, base->written);
        SDL_memset(base->buffer, 0, base->written);
        base->written = 0;
    }

    AudioFile *Jukebox::load_file(std::string filename) {
        if(!bank_.count(filename)) {
            AudioFile *file = new AudioFile(filename);
            if(!file->is_valid()) {
                delete file;
                file = nullptr;
            }
            bank_[filename] = file;
        }
        return bank_[filename];
    }

    void Jukebox::mix_raw(char *dst, char *src, int length, float volume) {
        for(int i = 0; i < length; i += 2) {
            float target = (dst[i+1] << 8) | (dst[i] & 0xFF);
            float sample = (src[i+1] << 8) | (src[i] & 0xFF);

            sample *= volume;
            target += sample;
            if(target > 32767) {
                target = 32767;
            }
            if(target < -32768) {
                target = -32768;
            }

            dst[i] = target;
            dst[i+1] = static_cast<short>(target) >> 8;
        }
    }

    bool Jukebox::is_playing() {
        return SDL_GetAudioDeviceStatus(device_) == SDL_AUDIO_PLAYING;
    }

    float Jukebox::get_volume() {
        return master_volume_;
    }

    Sound *Jukebox::load_sound(std::string filename) {
        AudioFile *file = load_file(filename);
        if(file == nullptr) {
            return nullptr;
        }
        vorbis_info *meta = ov_info(file->get_encoded(), -1);
        char buffer[4096];
        std::vector<char> temp;

        int eof = 0;
        while(!eof) {
            long bytes_read = ov_read(
                file->get_encoded(), 
                buffer, sizeof(buffer), 
                0, 2, 1, 
                nullptr
            );
            for(int i = 0; i < bytes_read; i++) {
                temp.push_back(buffer[i]);
            }
            if(bytes_read == 0) {
                eof = 1;
            }
            else if(bytes_read == OV_EBADLINK) {
                return nullptr;
            }
        }

        Sound *sound = new Sound();
        sound->length = temp.size();
        sound->samples = new char[sound->length];
        SDL_memcpy(sound->samples, &(temp[0]), sound->length);
        return sound;
    }

    void Jukebox::destroy_sound(Sound *sound) {
        delete[] sound->samples;
        delete sound;
    }

    void Jukebox::play_sound(Sound *sound, float volume) {
        chunks_.push_back(
            {{sound->samples, sound->length},
             volume}
        );
    }

    void Jukebox::set_volume(float volume) {
        if(volume > 1.0) {
            volume = 1.0;
        }
        if(volume <= 0) {
            volume = 0;
        }
        master_volume_ = volume;
    }

    void Jukebox::play() {
        SDL_PauseAudioDevice(device_, 0);
    }

    void Jukebox::pause() {
        SDL_PauseAudioDevice(device_, 1);
    }

    void Jukebox::mix_chunk(Chunk *chunk, int *max_copy) {
        int length = chunk->sound.length;
        if(length > base_.length - base_.written) {
            length = base_.length - base_.written;
        }
        if(*max_copy < length) {
            *max_copy = length;
        }

        mix_raw(
            base_.buffer + base_.written,
            chunk->sound.samples,
            length,
            chunk->volume
        );
        chunk->sound.samples += length;
        chunk->sound.length -= length;
    }

    void Jukebox::update() {
        if(!is_playing()) {
            return;
        }
        SDL_LockAudioDevice(device_);
        int max_copy = 0;
        auto it = chunks_.begin();
        while(it != chunks_.end()) {
            Chunk &chunk = *it;
            if(!chunk.sound.length) {
                it = chunks_.erase(it);
            }
            else {
                mix_chunk(&chunk, &max_copy);
                ++it;
            }
        }
        base_.written += max_copy;
        SDL_UnlockAudioDevice(device_);
    }
}