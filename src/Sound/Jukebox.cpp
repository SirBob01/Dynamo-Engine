#include "Jukebox.hpp"

namespace Dynamo::Sound {
    Jukebox::Jukebox(const std::string asset_directory) :
        _assets(asset_directory) {
        _volume = 1.0f;

        // Initialize state
        _input_stream = nullptr;
        _input_state.channels = 0;

        _output_stream = nullptr;
        _output_state.channels = 0;

        // Initialize PortAudio
        PaError err;
        err = Pa_Initialize();
        if (err != paNoError) {
            Log::error("Could not initialize PortAudio subsystem: {}",
                       Pa_GetErrorText(err));
        }

        // Set the default input and output devices
        const std::vector<Device> devices = get_devices();
        for (const Device &device : devices) {
            if (device.id == Pa_GetDefaultOutputDevice()) {
                set_output_device(device);
                break;
            }
        }
        for (const Device &device : devices) {
            if (device.id != Pa_GetDefaultInputDevice() &&
                device.input_channels > 0) {
                set_input_device(device);
                break;
            }
        }

        if (_output_stream == nullptr) {
            Log::error("Jukebox could not find a suitable output device.");
        }
    }

    Jukebox::~Jukebox() {
        PaError err;

        // Close the IO streams
        if (_input_stream) {
            err = Pa_CloseStream(_input_stream);
            if (err != paNoError) {
                Log::error("PortAudio could not close input stream: {}",
                           Pa_GetErrorText(err));
            }
        }
        if (_output_stream) {
            err = Pa_CloseStream(_output_stream);
            if (err != paNoError) {
                Log::error("PortAudio could not close output stream: {}",
                           Pa_GetErrorText(err));
            }
        }

        // Uninitialize PortAudio
        err = Pa_Terminate();
        if (err != paNoError) {
            Log::error("Could not terminate PortAudio subsystem: {}",
                       Pa_GetErrorText(err));
        }
    }

    i32 Jukebox::input_callback(const void *input,
                                void *output,
                                unsigned long frame_count,
                                const PaStreamCallbackTimeInfo *time_info,
                                PaStreamCallbackFlags status_flags,
                                void *data) {
        State &state = *static_cast<State *>(data);
        state.buffer.write(static_cast<const WaveSample *>(input),
                           frame_count * state.channels);
        return 0;
    }

    i32 Jukebox::output_callback(const void *input,
                                 void *output,
                                 unsigned long frame_count,
                                 const PaStreamCallbackTimeInfo *time_info,
                                 PaStreamCallbackFlags status_flags,
                                 void *data) {
        State &state = *static_cast<State *>(data);
        state.buffer.read(static_cast<WaveSample *>(output),
                          frame_count * state.channels);
        return 0;
    }

    void Jukebox::process_chunk(Chunk<StaticMaterial> &chunk) {
        Sound &sound = chunk.sound.get();
        StaticMaterial &material = chunk.material.get();

        // Calculate the number of frames in the destination
        f64 frame_stop = std::min(chunk.frame + MAX_CHUNK_LENGTH,
                                  static_cast<f32>(sound.frames()));
        f64 frames = frame_stop - chunk.frame;

        // Calculate the number of frames required in the original signal to
        // produce the destination frames
        f64 factor = sound.sample_rate() / _output_state.sample_rate;
        f64 length = frames * factor;

        // Resample the audio to the device sample rate
        Sound transformed(frames, _output_state.channels);
        for (u32 c = 0; c < transformed.channels(); c++) {
            resample_signal(sound[c],
                            transformed[c],
                            chunk.frame,
                            length,
                            sound.sample_rate(),
                            _output_state.sample_rate);
        }

        // Apply the filters
        for (const auto &filter : material.filters) {
            transformed = filter->apply(transformed, 0, transformed.frames());
        }

        // Mix the filtered sound onto the composite signal
        f32 volume = material.volume * _volume;
        for (u32 f = 0; f < transformed.frames(); f++) {
            for (u32 c = 0; c < transformed.channels(); c++) {
                u32 i = f * transformed.channels() + c;
                f32 s0 = _composite[i];
                f32 s1 = transformed[c][f];
                _composite[i] = (s0 + s1) * volume;
            }
        }

        // Advance chunk frame
        chunk.frame += length;
    }

    void Jukebox::process_chunk(Chunk<DynamicMaterial> &chunk) {
        if (_listeners.size() == 0) return;

        Sound &sound = chunk.sound.get();
        DynamicMaterial &material = chunk.material.get();

        // Calculate the number of frames in the destination
        f64 frame_stop = std::min(chunk.frame + MAX_CHUNK_LENGTH,
                                  static_cast<f32>(sound.frames()));
        f64 frames = frame_stop - chunk.frame;

        // Calculate the number of frames required in the original signal to
        // produce the destination frames
        f64 factor = sound.sample_rate() / _output_state.sample_rate;
        f64 length = frames * factor;

        // Resample the audio to the device sample rate
        Sound transformed(frames, _output_state.channels);
        for (u32 c = 0; c < transformed.channels(); c++) {
            resample_signal(sound[c],
                            transformed[c],
                            chunk.frame,
                            length,
                            sound.sample_rate(),
                            _output_state.sample_rate);
        }

        // Apply the filters
        ListenerProperties &listener = find_closest_listener(material);
        for (const auto &filter : material.filters) {
            transformed = filter->apply(transformed,
                                        0,
                                        transformed.frames(),
                                        material,
                                        listener);
        }

        // Mix the filtered sound onto the composite signal
        f32 volume = material.volume * listener.volume * _volume;
        for (u32 f = 0; f < transformed.frames(); f++) {
            for (u32 c = 0; c < transformed.channels(); c++) {
                u32 i = f * transformed.channels() + c;
                f32 s0 = _composite[i];
                f32 s1 = transformed[c][f];
                _composite[i] = (s0 + s1) * volume;
            }
        }

        // Advance chunk frame
        chunk.frame += length;
    }

    ListenerProperties &
    Jukebox::find_closest_listener(const DynamicMaterial &material) {
        u32 closest_index = 0;
        for (u32 i = 0; i < _listeners.size(); i++) {
            Vec3 best = _listeners[i].position;
            Vec3 curr = _listeners[closest_index].position;

            f32 a = (best - material.position).length_squared();
            f32 b = (curr - material.position).length_squared();
            if (b < a) {
                closest_index = i;
            }
        }
        return _listeners[closest_index];
    }

    const std::vector<Device> Jukebox::get_devices() {
        std::vector<Device> devices;
        PaError err;

        // Count the devices
        i32 device_count = Pa_GetDeviceCount();
        if (device_count < 0) {
            err = device_count;
            Log::error("PortAudio failed to count sound devices: {}",
                       Pa_GetErrorText(err));
        }

        // List all devices
        for (i32 index = 0; index < device_count; index++) {
            const PaDeviceInfo *device_info = Pa_GetDeviceInfo(index);
            Device device;
            device.id = index;
            device.name = device_info->name;
            device.input_channels = device_info->maxInputChannels;
            device.output_channels = device_info->maxOutputChannels;
            device.sample_rate = device_info->defaultSampleRate;
            devices.push_back(device);
        }

        return devices;
    }

    void Jukebox::set_input_device(const Device &device) {
        PaError err;
        if (_input_stream) {
            err = Pa_CloseStream(_input_stream);
            if (err != paNoError) {
                Log::error("PortAudio could not close input stream: {}",
                           Pa_GetErrorText(err));
            }
        }

        // Update internal state
        _input_state.channels = device.input_channels;

        // Open and start the stream
        PaStreamParameters params;
        params.channelCount = device.input_channels;
        params.device = device.id;
        params.hostApiSpecificStreamInfo = nullptr;
        params.sampleFormat = paFloat32;
        params.suggestedLatency = 0;

        err = Pa_OpenStream(&_input_stream,
                            &params,
                            nullptr,
                            device.sample_rate,
                            paFramesPerBufferUnspecified,
                            paNoFlag,
                            input_callback,
                            &_input_state);
        if (err != paNoError) {
            Log::error("Could not open PortAudio input stream: {}",
                       Pa_GetErrorText(err));
        }

        err = Pa_StartStream(_input_stream);
        if (err != paNoError) {
            Log::error("Could not start PortAudio input stream: {}",
                       Pa_GetErrorText(err));
        }

        const PaStreamInfo *info = Pa_GetStreamInfo(_input_stream);
        if (info == nullptr) {
            Log::error("Could not query information about the PortAudio input "
                       "stream.");
        }
        _input_state.sample_rate = info->sampleRate;
    }

    void Jukebox::set_output_device(const Device &device) {
        PaError err;
        if (_output_stream) {
            err = Pa_CloseStream(_output_stream);
            if (err != paNoError) {
                Log::error("PortAudio could not close output stream: {}",
                           Pa_GetErrorText(err));
            }
        }

        // Open and start the stream
        PaStreamParameters params;
        params.channelCount = device.output_channels;
        params.device = device.id;
        params.hostApiSpecificStreamInfo = nullptr;
        params.sampleFormat = paFloat32;
        params.suggestedLatency = 0;

        err = Pa_OpenStream(&_output_stream,
                            nullptr,
                            &params,
                            device.sample_rate,
                            paFramesPerBufferUnspecified,
                            paNoFlag,
                            output_callback,
                            &_output_state);
        if (err != paNoError) {
            Log::error("Could not open PortAudio output stream: {}",
                       Pa_GetErrorText(err));
        }

        err = Pa_StartStream(_output_stream);
        if (err != paNoError) {
            Log::error("Could not start PortAudio output stream: {}",
                       Pa_GetErrorText(err));
        }

        const PaStreamInfo *info = Pa_GetStreamInfo(_output_stream);
        if (info == nullptr) {
            Log::error("Could not query information about the PortAudio input "
                       "stream.");
        }

        // Update internal state
        _output_state.sample_rate = info->sampleRate;
        _output_state.channels = device.output_channels;
        _composite.resize(MAX_CHUNK_LENGTH * device.output_channels, 0);
    }

    b8 Jukebox::is_playing() {
        return _output_stream != nullptr && Pa_IsStreamActive(_output_stream);
    }

    b8 Jukebox::is_recording() {
        return _input_stream != nullptr && Pa_IsStreamActive(_input_stream);
    }

    f32 Jukebox::get_volume() { return _volume; }

    void Jukebox::pause() { Pa_StopStream(_output_stream); }

    void Jukebox::resume() { Pa_StartStream(_output_stream); }

    void Jukebox::set_volume(f32 volume) {
        _volume = std::clamp(volume, 0.0f, 1.0f);
    }

    SoundCache &Jukebox::get_sounds() { return _assets; }

    ListenerSet &Jukebox::get_listeners() { return _listeners; }

    HRTF &Jukebox::get_hrtf() { return _hrtf; }

    void Jukebox::play(Asset<Sound> &sound, StaticMaterial &material) {
        f32 frame = _output_state.sample_rate * material.start_seconds;
        _static_chunks.push_back({*sound, material, frame});
    }

    void Jukebox::play(Asset<Sound> &sound, DynamicMaterial &material) {
        f32 frame = _output_state.sample_rate * material.start_seconds;
        _dynamic_chunks.push_back({*sound, material, frame});
    }

    void Jukebox::update() {
        // Wait for there to be available space in the buffer
        u32 min_length = MAX_CHUNK_LENGTH * _output_state.channels;
        if (_output_state.buffer.remaining() < min_length || !is_playing()) {
            return;
        }

        // Zero-out the composite waveform
        std::fill(_composite.begin(), _composite.end(), 0);

        // Process static chunks and mix onto the composite
        auto s_it = _static_chunks.begin();
        while (s_it != _static_chunks.end()) {
            Chunk<StaticMaterial> &chunk = *s_it;
            if (chunk.frame >= chunk.sound.get().frames()) {
                s_it = _static_chunks.erase(s_it);
            } else {
                process_chunk(chunk);
                s_it++;
            }
        }

        // Process dynamic chunks and mix onto the composite
        auto d_it = _dynamic_chunks.begin();
        while (d_it != _dynamic_chunks.end()) {
            Chunk<DynamicMaterial> &chunk = *d_it;
            if (chunk.frame >= chunk.sound.get().frames()) {
                d_it = _dynamic_chunks.erase(d_it);
            } else {
                process_chunk(chunk);
                d_it++;
            }
        }

        // Write the composite to the output buffer
        _output_state.buffer.write(_composite.data(), _composite.size());
    }
} // namespace Dynamo::Sound