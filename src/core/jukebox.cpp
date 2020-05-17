#include "jukebox.h"

namespace Dynamo {
    Jukebox::AudioFile::AudioFile(std::string filename) {
        file = fopen(filename.c_str(), "rb");
        if(!file) {
            throw GenericError(filename + " file doesn't exist");
        }
        else if(ov_open_callbacks(file, &vb, nullptr, 
                                  0, OV_CALLBACKS_NOCLOSE) < 0) {
            throw GenericError(filename + " is not an Ogg Vorbis file");
        }
    }

    Jukebox::AudioFile::~AudioFile() {
        ov_clear(&vb);
        fclose(file);
    }

    OggVorbis_File *Jukebox::AudioFile::get_encoded() {
        return &vb;
    }

    Jukebox::StreamLine::StreamLine() : track(4096) {
        volume = 0;
        max_volume = 1.0;

        // So we can measure time starting from 0
        time = -1;
        loop_counter = 0;
        
        playing = true;
    }

    Jukebox::Jukebox() : base_(4096), record_(65536) {
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
        play();
    }

    Jukebox::~Jukebox() {
        clear();
        SDL_CloseAudioDevice(output_);
        SDL_CloseAudioDevice(input_);
    }

    void Jukebox::play_callback(void *data, uint8_t *stream, int length) {
        Sound &base = *static_cast<Sound *>(data);
        int written = base.size();
        SDL_memset(stream+written, 0, length-written);
        SDL_memcpy(stream, &base[0], written);
        base.clear();
    }

    void Jukebox::record_callback(void *data, uint8_t *stream, int length) {
        RingBuffer &record = *static_cast<RingBuffer *>(data);
        if(record.is_full()) {
            record.clear();
        }
        for(int i = 0; i < length; i++) {
            record.write(stream[i]);
        }
    }

    void Jukebox::load_file(std::string filename) {
        if(!bank_.count(filename)) {
            bank_[filename] = std::make_shared<AudioFile>(filename);
        }
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

    void Jukebox::mix_chunk(Chunk &chunk, int current_size) {
        int length = std::min(
            chunk.sound->size() - chunk.written,
            base_.capacity() - current_size
        );
        if(current_size + length > base_.size()) {
            base_.resize(current_size + length, 0);
        }

        mix_raw(
            &base_[0] + current_size,
            &(*chunk.sound)[0] + chunk.written,
            length,
            chunk.volume
        );
        chunk.written += length;
    }

    void Jukebox::mix_stream(StreamLine &stream, int current_size) {
        int length = std::min(
            stream.track.size(),
            base_.capacity() - current_size
        );
        if(current_size + length > base_.size()) {
            base_.resize(current_size + length, 0);
        }

        mix_raw(
            &base_[0] + current_size,
            &stream.track[0],
            length,
            stream.volume
        );
        stream.track.resize(stream.track.size() - length);
    }

    void Jukebox::check_stream_validity(SoundStream stream) {
        if(stream >= streams_.size()) {
            throw InvalidKey(
                "Stream ID "+std::to_string(stream),
                "streams_");
        }
    }

    bool Jukebox::is_playing() {
        return SDL_GetAudioDeviceStatus(output_) == SDL_AUDIO_PLAYING;
    }

    bool Jukebox::is_recording() {
        return SDL_GetAudioDeviceStatus(input_) == SDL_AUDIO_PLAYING;
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

    SoundStream Jukebox::generate_stream() {
        streams_.emplace_back();
        return streams_.size() - 1;
    }

    bool Jukebox::is_stream_playing(SoundStream stream) {
        check_stream_validity(stream);
        return streams_[stream].playing;
    }

    bool Jukebox::is_stream_empty(SoundStream stream) {
        check_stream_validity(stream);
        return streams_[stream].queue.size() == 0;
    }

    bool Jukebox::is_stream_transition(SoundStream stream) {
        check_stream_validity(stream);
        return streams_[stream].time == -1;
    }

    float Jukebox::get_stream_volume(SoundStream stream) {
        check_stream_validity(stream);
        return streams_[stream].max_volume;
    }

    void Jukebox::set_stream_volume(SoundStream stream, float volume) {
        check_stream_validity(stream);
        if(volume > 1.0) {
            volume = 1.0;
        }
        if(volume <= 0) {
            volume = 0;
        }
        streams_[stream].max_volume = volume;
    }

    void Jukebox::queue_stream(std::string filename, SoundStream stream,
                               double fadein, double fadeout,
                               int loops) {
        check_stream_validity(stream);
        load_file(filename);
        auto &file = bank_[filename];
        double duration = ov_time_total(file->get_encoded(), -1);

        streams_[stream].queue.push({
            file,
            duration,
            fadein,
            fadeout,
            loops
        });
    }

    void Jukebox::play_stream(SoundStream stream) {
        check_stream_validity(stream);
        streams_[stream].playing = true;
    }

    void Jukebox::pause_stream(SoundStream stream) {
        check_stream_validity(stream);
        streams_[stream].playing = false;
    }

    void Jukebox::skip_stream(SoundStream stream, double fadeout) {
        check_stream_validity(stream);
        auto &stream_line = streams_[stream];
        if(is_stream_empty(stream) || is_stream_transition(stream)) {
            return;
        }
        StreamQueued &current = stream_line.queue.front();
        
        // Seamless transition if fading out in the middle of fade in
        float t = stream_line.volume / stream_line.max_volume;
        current.fadeout = fadeout;
        current.duration = stream_line.time + (fadeout * t);
    }

    void Jukebox::clear_stream(SoundStream stream) {
        check_stream_validity(stream);
        auto &stream_line = streams_[stream];
        if(is_stream_empty(stream) || is_stream_transition(stream)) {
            return;
        }

        // Reset stream data
        stream_line.volume = 0;
        stream_line.time = -1;
        stream_line.loop_counter = 0;

        // Reset the seek of each track in the stream before emptying
        while(!stream_line.queue.empty()) {
            StreamQueued &current = stream_line.queue.front();
            ov_raw_seek(
                current.file->get_encoded(), 
                0
            );
            stream_line.queue.pop();
        }
    }

    Sound Jukebox::load_sound(std::string filename) {
        Sound sound;

        load_file(filename);
        auto &file = bank_[filename];
        vorbis_info *meta = ov_info(file->get_encoded(), -1);

        char buffer[4096];
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
                sound.emplace_back(buffer[i]);
            }
            if(bytes_read == 0) {
                eof = 1;
            }
            else if(bytes_read == OV_EBADLINK) {
                return sound;
            }
        }
        return sound;
    }

    void Jukebox::play_sound(Sound &sound, float volume) {
        chunks_.emplace_back(Chunk {&sound, 0, volume});
    }

    void Jukebox::stream_recorded(Sound &target) {
        while(!record_.is_empty()) {
            target.emplace_back(record_.read());
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

    void Jukebox::update(unsigned dt) {
        if(!is_playing()) {
            return;
        }
        // Lock device to prevent race conditions
        SDL_LockAudioDevice(output_);
        int current_size = base_.size();

        // Update streaming tracks
        for(auto &stream : streams_) {
            if(!stream.playing || !stream.queue.size()) {
                continue;
            }

            StreamQueued &current = stream.queue.front();
            OggVorbis_File *ogg = current.file->get_encoded();
            if(stream.time < 0) {
                stream.time = 0;
            }
            stream.time += dt / 1000.0;

            // Fade in and out of stream track
            if(!current.fadein) {
                stream.volume = stream.max_volume;
            }
            else if(stream.time <= current.fadein) {
                stream.volume = Util::lerp(
                    0.0f, stream.max_volume,
                    1.0 - (current.fadein - stream.time) / current.fadein
                );
            }

            if(stream.time >= current.duration - current.fadeout) {
                stream.volume = Util::lerp(
                    stream.max_volume, 0.0f,
                    (stream.time - current.duration + current.fadeout) /
                                                      current.fadeout
                );
            }
            
            // Write to stream buffer
            while(stream.track.size() < stream.track.capacity()) {
                char buffer[4096];
                long bytes_read = ov_read(
                    ogg, 
                    buffer,
                    stream.track.capacity() - stream.track.size(), 
                    0, 2, 1, 
                    nullptr
                );
                
                // EOF
                if(!bytes_read) {
                    break;
                }

                for(int i = 0; i < bytes_read; i++) {
                    stream.track.emplace_back(buffer[i]);
                }
            }

            // End of track... loop or change?
            if(stream.time >= current.duration) {
                ov_raw_seek(ogg, 0);
                current.duration = ov_time_total(ogg, -1);
                
                stream.volume = 0;
                stream.time = -1;
                if(++stream.loop_counter == current.loops) {
                    stream.loop_counter = 0;
                    stream.queue.pop();
                }
            }
            else {
                mix_stream(stream, current_size);
            }
        }

        // Mix all active chunks before pushing to device buffer
        auto it = chunks_.begin();
        while(it != chunks_.end()) {
            Chunk &chunk = *it;
            if(chunk.written == chunk.sound->size()) {
                it = chunks_.erase(it);
            }
            else {
                mix_chunk(chunk, current_size);
                ++it;
            }
        }

        SDL_UnlockAudioDevice(output_);
    }

    void Jukebox::clear() {
        for(auto &file : bank_) {
            ov_raw_seek(file.second->get_encoded(), 0);
        }
        chunks_.clear();
        streams_.clear();
    }
}