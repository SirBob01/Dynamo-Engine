#include <Math/Vectorize.hpp>
#include <Sound/DSP/Resample.hpp>
#include <Sound/Jukebox.hpp>

namespace Dynamo::Sound {
    Jukebox::Jukebox() {
        // Initialize state
        _input_stream = nullptr;
        _input_state.channels = 0;

        _output_stream = nullptr;
        _output_state.channels = 0;

        _volume = 1.0f;

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

    int Jukebox::input_callback(const void *input,
                                void *output,
                                unsigned long frame_count,
                                const PaStreamCallbackTimeInfo *time_info,
                                PaStreamCallbackFlags status_flags,
                                void *data) {
        PaState *state = static_cast<PaState *>(data);
        state->buffer.write(static_cast<const WaveSample *>(input),
                            frame_count * state->channels);
        return 0;
    }

    int Jukebox::output_callback(const void *input,
                                 void *output,
                                 unsigned long frame_count,
                                 const PaStreamCallbackTimeInfo *time_info,
                                 PaStreamCallbackFlags status_flags,
                                 void *data) {
        PaState *state = static_cast<PaState *>(data);
        state->buffer.read(static_cast<WaveSample *>(output),
                           frame_count * state->channels);
        return 0;
    }

    Listener &Jukebox::find_closest_listener(Vec3 position) {
        unsigned closest_index = 0;
        float closest_distance = std::numeric_limits<float>::max();
        for (unsigned i = 0; i < _listeners.size(); i++) {
            const Listener &listener = _listeners[i];
            float distance = (listener.position - position).length_squared();
            if (distance < closest_distance) {
                closest_index = i;
                closest_distance = distance;
            }
        }
        return _listeners[closest_index];
    }

    void Jukebox::process_chunk(Chunk &chunk) {
        // Jukebox requires at least 1 listener for playback
        if (_listeners.empty()) return;

        Sound &sound = chunk.sound;
        Material &material = chunk.material;

        unsigned src_frames = sound.frames();
        unsigned src_channels = sound.channels();

        // Calculate the number of frames in the destination
        double frame_stop = std::min(chunk.frame + MAX_CHUNK_LENGTH,
                                     static_cast<float>(src_frames));
        double frames = frame_stop - chunk.frame;

        // Calculate the number of frames required in the original signal to
        // produce the destination frames
        double factor = sound.sample_rate() / _output_state.sample_rate;
        double length = frames * factor;

        // Resample the audio to the device sample rate
        _scratch.resize(frames, src_channels);
        for (unsigned c = 0; c < src_channels; c++) {
            resample_signal(sound[c],
                            _scratch[c],
                            chunk.frame,
                            length,
                            sound.sample_rate(),
                            _output_state.sample_rate);
        }

        // Apply the filters, if any
        Listener &listener = find_closest_listener(material.position);
        if (chunk.filter.has_value()) {
            // TODO: Rethink the Filter interface, minimize memcopies
            _scratch = chunk.filter->get().apply(_scratch,
                                                 0,
                                                 _scratch.frames(),
                                                 material,
                                                 listener);
        }

        // Remix to the output device channels
        _remixed.resize(_scratch.frames(), _output_state.channels);
        _remixed.clear();
        _scratch.remix(_remixed, 0, 0, _scratch.frames());

        // Mix the processed sound onto the composite signal
        float volume = material.volume * listener.volume * _volume;
        for (unsigned c = 0; c < _composite.channels(); c++) {
            Vectorize::vsma(_remixed[c],
                            volume,
                            _composite[c],
                            _composite.frames());
        }

        // Advance chunk frame
        chunk.frame += length;
    }

    const std::vector<Device> Jukebox::get_devices() {
        std::vector<Device> devices;
        PaError err;

        // Count the devices
        int device_count = Pa_GetDeviceCount();
        if (device_count < 0) {
            err = device_count;
            Log::error("PortAudio failed to count sound devices: {}",
                       Pa_GetErrorText(err));
        }

        // List all devices
        for (int index = 0; index < device_count; index++) {
            const PaDeviceInfo *device_info = Pa_GetDeviceInfo(index);
            Device device;
            device.id = index;
            device.name = device_info->name;
            device.input_channels = device_info->maxInputChannels;
            device.output_channels = device_info->maxOutputChannels;
            device.sample_rate = device_info->defaultSampleRate;
            device.input_latency = device_info->defaultLowInputLatency;
            device.output_latency = device_info->defaultLowOutputLatency;

            devices.push_back(device);
        }

        return devices;
    }

    void Jukebox::add_listener(Listener &listener) {
        _listeners.push_back(listener);
    }

    bool Jukebox::remove_listener(Listener &listener) {
        auto it = _listeners.begin();
        while (it != _listeners.end()) {
            if (&(*it).get() == &listener) {
                _listeners.erase(it);
                return true;
            }
            it++;
        }
        return false;
    }

    void Jukebox::clear_listeners() { _listeners.clear(); }

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
        params.suggestedLatency = device.input_latency;

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
        params.suggestedLatency = device.output_latency;

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
        _composite.resize(MAX_CHUNK_LENGTH, device.output_channels);
    }

    bool Jukebox::is_playing() {
        return _output_stream != nullptr && Pa_IsStreamActive(_output_stream);
    }

    bool Jukebox::is_recording() {
        return _input_stream != nullptr && Pa_IsStreamActive(_input_stream);
    }

    float Jukebox::get_volume() { return _volume; }

    void Jukebox::set_volume(float volume) {
        _volume = std::clamp(volume, 0.0f, 1.0f);
    }

    void Jukebox::play(Sound &sound,
                       Material &material,
                       std::optional<FilterRef> filter) {
        float frame = _output_state.sample_rate * material.start_seconds;
        _chunks.push_back({sound, material, filter, frame});
    }

    void Jukebox::pause() { Pa_StopStream(_output_stream); }

    void Jukebox::resume() { Pa_StartStream(_output_stream); }

    void Jukebox::update() {
        // Wait for there to be available space in the buffer
        unsigned min_length = MAX_CHUNK_LENGTH * _output_state.channels;
        if (_output_state.buffer.remaining() < min_length || !is_playing()) {
            return;
        }

        // Zero-out the composite waveform
        _composite.clear();

        // Process chunks and mix onto the composite
        auto d_it = _chunks.begin();
        while (d_it != _chunks.end()) {
            Chunk &chunk = *d_it;
            if (chunk.frame >= chunk.sound.get().frames()) {
                d_it = _chunks.erase(d_it);
            } else {
                process_chunk(chunk);
                d_it++;
            }
        }

        // Clamp channels
        for (unsigned c = 0; c < _composite.channels(); c++) {
            Vectorize::vclip(_composite[c],
                             -1,
                             1,
                             _composite[c],
                             _composite.frames());
        }

        // Interleave the composite and write to the ring buffer
        for (unsigned f = 0; f < _composite.frames(); f++) {
            for (unsigned c = 0; c < _composite.channels(); c++) {
                _output_state.buffer.write(_composite[c][f]);
            }
        }
    }
} // namespace Dynamo::Sound