#include "jukebox.h"

namespace Dynamo {
    AudioFile::AudioFile(std::string filename) {
        file = fopen(filename.c_str(), "rb");
        if(!file) {
            throw GenericError(filename + " file doesn't exist");
        }
        else if(ov_open_callbacks(file, &vb, nullptr, 
                                  0, OV_CALLBACKS_NOCLOSE) < 0) {
            throw GenericError(filename + " is not an Ogg Vorbis file");
        }
    }

    AudioFile::~AudioFile() {
        ov_clear(&vb);
        fclose(file);
    }

    OggVorbis_File *AudioFile::get_encoded() {
        return &vb;
    }

    Track::Track() {
        length = 4096;
        buffer = new char[length];
        SDL_memset(buffer, 0, length);

        written = 0;
    }

    Track::~Track() {
        delete[] buffer;
    }

    Stream::Stream() {
        volume = 1.0;
        fadeout = 0;
        fadein = 0;
        playing = true;
    }

    Jukebox::Jukebox() {
        // Initialize audio device
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
        master_volume_ = 1.0;
        play();
    }

    Jukebox::~Jukebox() {
        clear();
        for(auto &pair : bank_) {
            delete pair.second;
        }
        SDL_CloseAudioDevice(device_);
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
            bank_[filename] = file;
        }
        return bank_[filename];
    }

    void Jukebox::mix_raw(char *dst, char *src, int length, float volume) {
        for(int i = 0; i < length; i += 2) {
            float target = (dst[i+1] << 8) | (dst[i] & 0xFF);
            float sample = (src[i+1] << 8) | (src[i] & 0xFF);

            sample *= (volume * master_volume_);
            target += sample;

            // Clip to prevent distortion
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

    void Jukebox::mix_stream(Stream *stream, int *max_copy) {
        int length = stream->track.written;
        if(length > base_.length - base_.written) {
            length = base_.length - base_.written;
        }
        if(*max_copy < length) {
            *max_copy = length;
        }

        mix_raw(
            base_.buffer + base_.written,
            stream->track.buffer,
            length,
            stream->volume
        );
        stream->track.written -= length;
    }

    void Jukebox::check_stream_validity(int stream) {
        if(stream >= streams_.size()) {
            throw InvalidKey(
                "Stream ID "+std::to_string(stream),
                "streams_");
        }
    }

    bool Jukebox::is_playing() {
        return SDL_GetAudioDeviceStatus(device_) == SDL_AUDIO_PLAYING;
    }

    float Jukebox::get_volume() {
        return master_volume_;
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

    int Jukebox::generate_stream() {
        streams_.push_back(new Stream());
        return streams_.size() - 1;
    }

    float Jukebox::get_stream_volume(int stream) {
        check_stream_validity(stream);
        return streams_[stream]->volume;
    }

    void Jukebox::set_stream_volume(int stream, float volume) {
        check_stream_validity(stream);
        if(volume > 1.0) {
            volume = 1.0;
        }
        if(volume <= 0) {
            volume = 0;
        }
        streams_[stream]->volume = volume;
    }

    void Jukebox::queue_stream(std::string filename, int stream,
                               int fadein_ms, int fadeout_ms) {
        check_stream_validity(stream);
        streams_[stream]->queue.push(load_file(filename));
    }

    void Jukebox::play_stream(int stream, int fadein_ms) {
        check_stream_validity(stream);
        streams_[stream]->playing = true;
    }

    void Jukebox::pause_stream(int stream, int fadeout_ms) {
        check_stream_validity(stream);
        streams_[stream]->playing = false;
    }

    void Jukebox::skip_stream(int stream, int fadeout_ms) {
        check_stream_validity(stream);
        if(streams_[stream]->queue.size()) {
            streams_[stream]->queue.pop();
        }
    }

    void Jukebox::clear_stream(int stream) {
        check_stream_validity(stream);
        streams_[stream]->queue = {};
    }

    Sound *Jukebox::load_sound(std::string filename) {
        AudioFile *file = load_file(filename);
        vorbis_info *meta = ov_info(file->get_encoded(), -1);
        char buffer[4096];
        std::vector<char> temp;

        ov_raw_seek(file->get_encoded(), 0);
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
        if(!sound) {
            return;
        }
        chunks_.push_back(
            {{sound->samples, sound->length},
             volume}
        );
    }

    void Jukebox::play() {
        SDL_PauseAudioDevice(device_, 0);
    }

    void Jukebox::pause() {
        SDL_PauseAudioDevice(device_, 1);
    }

    void Jukebox::update() {
        if(!is_playing()) {
            return;
        }
        // Lock device to prevent race conditions
        SDL_LockAudioDevice(device_);
        int max_copy = 0;

        // Update streaming tracks
        for(auto &stream : streams_) {
            if(!stream->playing || !stream->queue.size()) {
                continue;
            }
            AudioFile *current = stream->queue.front();

            while(stream->track.written < stream->track.length) {
                long bytes_read = ov_read(
                    current->get_encoded(), 
                    stream->track.buffer + stream->track.written,
                    stream->track.length - stream->track.written, 
                    0, 2, 1, 
                    nullptr
                );
                stream->track.written += bytes_read;
                
                if(!bytes_read) {
                    ov_raw_seek(current->get_encoded(), 0);
                    stream->queue.pop();
                }
            }
            mix_stream(stream, &max_copy);
        }

        // Mix all active chunks before pushing to device buffer
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

        // Only update written bytes after all chunks are mixed
        base_.written += max_copy;
        SDL_UnlockAudioDevice(device_);
    }

    void Jukebox::clear() {
        chunks_.clear();
        for(auto &stream : streams_) {
            delete stream;
        }
        streams_.clear();
    }
}