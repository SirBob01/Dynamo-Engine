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

    Sound::Sound(int size) {
        length = size;
        samples = new char[length];
        SDL_memset(samples, 0, length);
        write = 0;
    }

    Sound::~Sound() {
        delete[] samples;
    }

    Stream::Stream() : track(4096) {
        volume = 0;
        max_volume = 1.0;

        // So we can measure time starting from 0
        time = -1;
        loop_counter = 0;
        
        playing = true;
    }

    Jukebox::Jukebox(Clock *clock) : base_(4096), record_(65536) {
        // Initialize audio output device specifications
        SDL_AudioSpec desired_output;
        SDL_zero(desired_output);

        desired_output.channels = 2;
        desired_output.freq = 44100;
        desired_output.format = AUDIO_S16LSB;
        desired_output.samples = 1024;
        desired_output.userdata = &base_;
        desired_output.callback = play_callback;

        // Initialize audio input device specifications
        SDL_AudioSpec desired_input;
        SDL_zero(desired_input);
        
        desired_input.channels = 2;
        desired_input.freq = 44100;
        desired_input.format = AUDIO_S16LSB;
        desired_input.samples = 1024;
        desired_input.userdata = &record_;
        desired_input.callback = record_callback;

        output_ = SDL_OpenAudioDevice(
            nullptr, 0, &desired_output, &output_spec_, 0
        );
        input_ = SDL_OpenAudioDevice(
            nullptr, true, &desired_input, &input_spec_,
            SDL_AUDIO_ALLOW_FORMAT_CHANGE
        );

        master_volume_ = 1.0;
        clock_ = clock;
        play();
    }

    Jukebox::~Jukebox() {
        clear();
        for(auto &pair : bank_) {
            delete pair.second;
        }
        SDL_CloseAudioDevice(output_);
        SDL_CloseAudioDevice(input_);
    }

    void Jukebox::play_callback(void *data, uint8_t *stream, int length) {
        Sound *base = static_cast<Sound *>(data);
        SDL_memset(stream+base->write, 0, length-base->write);
        SDL_memcpy(stream, base->samples, base->write);
        SDL_memset(base->samples, 0, base->write);
        base->write = 0;
    }

    void Jukebox::record_callback(void *data, uint8_t *stream, int length) {
        RingBuffer *record = static_cast<RingBuffer *>(data);
        if(record->is_full()) {
            record->clear();
        }
        for(int i = 0; i < length; i++) {
            record->write(stream[i]);
        }
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
            target = Util::clamp(target, -32768.0f, 32767.0f);

            dst[i] = target;
            dst[i+1] = static_cast<short>(target) >> 8;
        }
    }

    void Jukebox::mix_chunk(Chunk &chunk, int *max_copy) {
        int length = chunk.length;
        if(length > base_.length - base_.write) {
            length = base_.length - base_.write;
        }
        if(*max_copy < length) {
            *max_copy = length;
        }

        mix_raw(
            base_.samples + base_.write,
            chunk.samples,
            length,
            chunk.volume
        );
        chunk.samples += length;
        chunk.length -= length;
    }

    void Jukebox::mix_stream(Stream &stream, int *max_copy) {
        int length = stream.track.write;
        if(length > base_.length - base_.write) {
            length = base_.length - base_.write;
        }
        if(*max_copy < length) {
            *max_copy = length;
        }

        mix_raw(
            base_.samples + base_.write,
            stream.track.samples,
            length,
            stream.volume
        );
        stream.track.write -= length;
    }

    void Jukebox::check_stream_validity(StreamID stream) {
        if(stream >= streams_.size()) {
            throw InvalidKey(
                "Stream ID "+std::to_string(stream),
                "streams_");
        }
    }

    bool Jukebox::is_playing() {
        return SDL_GetAudioDeviceStatus(output_) == SDL_AUDIO_PLAYING;
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

    StreamID Jukebox::generate_stream() {
        streams_.push_back(new Stream());
        return streams_.size() - 1;
    }

    bool Jukebox::is_stream_playing(StreamID stream) {
        check_stream_validity(stream);
        return streams_[stream]->playing;
    }

    bool Jukebox::is_stream_empty(StreamID stream) {
        check_stream_validity(stream);
        return streams_[stream]->queue.size() == 0;
    }

    bool Jukebox::is_stream_transition(StreamID stream) {
        check_stream_validity(stream);
        return streams_[stream]->time == -1;
    }

    float Jukebox::get_stream_volume(StreamID stream) {
        check_stream_validity(stream);
        return streams_[stream]->max_volume;
    }

    void Jukebox::set_stream_volume(StreamID stream, float volume) {
        check_stream_validity(stream);
        if(volume > 1.0) {
            volume = 1.0;
        }
        if(volume <= 0) {
            volume = 0;
        }
        streams_[stream]->max_volume = volume;
    }

    void Jukebox::queue_stream(std::string filename, StreamID stream,
                               double fadein, double fadeout,
                               int loops) {
        check_stream_validity(stream);
        AudioFile *file = load_file(filename);
        double duration = ov_time_total(file->get_encoded(), -1);

        streams_[stream]->queue.push({
            file,
            duration,
            fadein,
            fadeout,
            loops,
            0,
        });
    }

    void Jukebox::play_stream(StreamID stream) {
        check_stream_validity(stream);
        streams_[stream]->playing = true;
    }

    void Jukebox::pause_stream(StreamID stream) {
        check_stream_validity(stream);
        streams_[stream]->playing = false;
    }

    void Jukebox::skip_stream(StreamID stream, double fadeout) {
        check_stream_validity(stream);
        Stream *stream_ptr = streams_[stream];
        if(is_stream_empty(stream) || is_stream_transition(stream)) {
            return;
        }
        StreamMeta &current = stream_ptr->queue.front();
        
        // Seamless transition if fading out in the middle of fade in
        float t = stream_ptr->volume / stream_ptr->max_volume;
        current.fadeout = fadeout;
        current.duration = stream_ptr->time + (fadeout * t);
    }

    void Jukebox::clear_stream(StreamID stream) {
        check_stream_validity(stream);
        Stream *stream_ptr = streams_[stream];
        if(is_stream_empty(stream) || is_stream_transition(stream)) {
            return;
        }

        // Reset stream data
        stream_ptr->volume = 0;
        stream_ptr->time = -1;
        stream_ptr->loop_counter = 0;

        // Reset the seek of each track in the stream before emptying
        while(!stream_ptr->queue.empty()) {
            StreamMeta &current = stream_ptr->queue.front();
            ov_raw_seek(
                current.file->get_encoded(), 
                0
            );
            stream_ptr->queue.pop();
        }
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

        Sound *sound = new Sound(temp.size());
        sound->write = sound->length;
        SDL_memcpy(sound->samples, &(temp[0]), sound->length);
        return sound;
    }

    void Jukebox::play_sound(Sound *sound, float volume) {
        if(!sound) {
            return;
        }
        chunks_.push_back({
            sound->samples, 
            sound->length, 
            volume
        });
    }

    void Jukebox::stream_recorded(Sound *target) {
        while(target->write < target->length && !record_.is_empty()) {
            target->samples[target->write++] = record_.read();
        }
    }

    void Jukebox::play() {
        SDL_PauseAudioDevice(output_, 0);
    }

    void Jukebox::pause() {
        SDL_PauseAudioDevice(output_, 1);
    }

    void Jukebox::start_record() {
        SDL_PauseAudioDevice(input_, 0);
    }

    void Jukebox::pause_record() {
        SDL_PauseAudioDevice(input_, 1);
    }

    void Jukebox::update() {
        if(!is_playing()) {
            return;
        }
        // Lock device to prevent race conditions
        SDL_LockAudioDevice(output_);
        int max_copy = 0;

        // Update streaming tracks
        for(auto &stream : streams_) {
            if(!stream->playing || !stream->queue.size()) {
                continue;
            }

            StreamMeta &current = stream->queue.front();
            OggVorbis_File *ogg = current.file->get_encoded();
            if(stream->time < 0) {
                current.start_time = clock_->get_time();
            }

            // Fade in and out of stream track
            stream->time = (clock_->get_time() - current.start_time) / 1000.0;
            if(!current.fadein) {
                stream->volume = stream->max_volume;
            }
            else if(stream->time <= current.fadein) {
                stream->volume = Util::lerp(
                    0.0f, stream->max_volume,
                    1.0 - (current.fadein - stream->time) / current.fadein
                );
            }

            if(stream->time >= current.duration - current.fadeout) {
                stream->volume = Util::lerp(
                    stream->max_volume, 0.0f,
                    (stream->time - current.duration + current.fadeout) /
                                                       current.fadeout
                );
            }
            
            // Write to stream buffer
            while(stream->track.write < stream->track.length) {
                long bytes_read = ov_read(
                    ogg, 
                    stream->track.samples + stream->track.write,
                    stream->track.length - stream->track.write, 
                    0, 2, 1, 
                    nullptr
                );
                stream->track.write += bytes_read;
                
                // EOF
                if(!bytes_read) {
                    break;
                }
            }

            // End of track... loop or change?
            if(stream->time >= current.duration) {
                ov_raw_seek(ogg, 0);
                current.duration = ov_time_total(ogg, -1);
                
                stream->volume = 0;
                stream->time = -1;
                if(++stream->loop_counter == current.loops) {
                    stream->loop_counter = 0;
                    stream->queue.pop();
                }
            }
            else {
                mix_stream(*stream, &max_copy);
            }
        }

        // Mix all active chunks before pushing to device buffer
        auto it = chunks_.begin();
        while(it != chunks_.end()) {
            Chunk &chunk = *it;
            if(!chunk.length) {
                it = chunks_.erase(it);
            }
            else {
                mix_chunk(chunk, &max_copy);
                ++it;
            }
        }

        // Only update write bytes after all chunks are mixed
        base_.write += max_copy;
        SDL_UnlockAudioDevice(output_);
    }

    void Jukebox::clear() {
        for(auto &stream : streams_) {
            delete stream;
        }
        for(auto &file : bank_) {
            ov_raw_seek(file.second->get_encoded(), 0);
        }
        chunks_.clear();
        streams_.clear();
    }
}