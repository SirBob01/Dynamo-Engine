#include "jukebox.h"
#include <iostream>
namespace Dynamo {
    Jukebox::AudioFile::AudioFile(std::string filename) {
        vb = stb_vorbis_open_filename(filename.c_str(), nullptr, nullptr);
        if(vb == nullptr) {
            throw GenericError("Couldn't load Ogg Vorbis: " + filename);
        }
    }

    Jukebox::AudioFile::~AudioFile() {
        stb_vorbis_close(vb);
    }

    stb_vorbis *Jukebox::AudioFile::get_vorbis() {
        return vb;
    }

    Jukebox::StreamLine::StreamLine() : track(2048) {
        volume = 0;
        max_volume = 1.0;
        position = {0, 0};

        // So we can measure time starting from 0
        time = -1;
        loop_counter = 0;
        
        playing = true;
    }

    Jukebox::Jukebox() : base_(2048), record_(65536) {
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
            nullptr, 1, &desired_input, &input_spec_, 0
        );

        master_volume_ = 1.0;
        max_distance_ = 250.0f;
        play();
    }

    Jukebox::~Jukebox() {
        clear();
        SDL_CloseAudioDevice(output_);
        SDL_CloseAudioDevice(input_);
    }

    void Jukebox::play_callback(void *data, uint8_t *stream, int length) {
        Sound &base = *static_cast<Sound *>(data);
        int written = base.size() * sizeof(short);
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

    void Jukebox::mix_raw(short *dst, short *src, int length, 
                          float volume, Vec2D position) {
        // Distance attenuation
        float intensity;
        if(position.x == 0) {
            intensity = 1.0f;
        }
        else {
            intensity = Util::clamp(max_distance_/std::fabs(position.x), 0.0f, 1.0f);
        }
        
        // 2D stereo panning
        Vec2D dir = position.normalize();
        float right_v = (1.0 + dir.x)/2.0;
        float left_v = std::fabs((-1.0 + dir.x)/2.0);
        
        bool left_channel = true;
        for(int i = 0; i < length; i++) {
            float target = dst[i];
            float sample = src[i];

            // Adjust volume and clip to prevent distortion
            if(left_channel) {
                sample *= (intensity * volume * master_volume_ * left_v);
            }
            else {
                sample *= (intensity * volume * master_volume_ * right_v);            
            }
            
            // Add the final sample into the mix
            target += sample;
            target = Util::clamp(target, -32768.0f, 32767.0f);

            dst[i] = static_cast<short>(target);
            left_channel = !left_channel;
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
            chunk.volume,
            chunk.position
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
            stream.volume,
            stream.position
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

    float Jukebox::get_max_distance() {
        return max_distance_;
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

    void Jukebox::set_max_distance(float distance) {
        max_distance_ = distance;
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

    void Jukebox::set_stream_position(SoundStream stream, Vec2D position) {
        check_stream_validity(stream);
        streams_[stream].position = position;
    }

    void Jukebox::queue_stream(std::string filename, SoundStream stream,
                               double fadein, double fadeout,
                               int loops) {
        check_stream_validity(stream);
        load_file(filename);
        auto &file = bank_[filename];
        double duration = stb_vorbis_stream_length_in_seconds(
            file->get_vorbis()
        );

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
            stb_vorbis_seek_start(current.file->get_vorbis());
            stream_line.queue.pop();
        }
    }

    Sound Jukebox::load_sound(std::string filename) {
        Sound sound;

        load_file(filename);
        auto &file = bank_[filename];

        short buffer[2048];
        stb_vorbis_seek_start(file->get_vorbis());
        int read = -1;
        while(read) {
            read = stb_vorbis_get_samples_short_interleaved(
                file->get_vorbis(), 2,
                buffer,
                2048
            );
            for(int i = 0; i < read * 2; i++) {
                sound.emplace_back(buffer[i]);
            }
        }
        return sound;
    }

    void Jukebox::play_sound(Sound &sound, float volume, Vec2D position) {
        chunks_.emplace_back(Chunk {&sound, 0, volume, position});
    }

    void Jukebox::stream_recorded(Sound &target) {
        while(!record_.is_empty()) {
            char l = record_.read();
            char r = record_.read();
            short pcm = (r << 8) | (l & 0xFF);
            target.emplace_back(pcm);
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
            stb_vorbis *ogg = current.file->get_vorbis();
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
                short buffer[2048];
                int request = stream.track.capacity() - stream.track.size();
                int samples = stb_vorbis_get_samples_short_interleaved(
                    ogg, 2, buffer, request
                );
                // EOF
                if(!samples) {
                    break;
                }
                for(int i = 0; i < samples * 2; i++) {
                    stream.track.emplace_back(buffer[i]);
                }
            }

            // End of track... loop or change?
            if(stream.time >= current.duration) {
                stb_vorbis_seek_start(ogg);
                current.duration = stb_vorbis_stream_length_in_seconds(ogg);
                
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
            stb_vorbis_seek_start(file.second->get_vorbis());
        }
        chunks_.clear();
        streams_.clear();
    }
}