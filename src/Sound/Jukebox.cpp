#include <Math/Vectorize.hpp>
#include <Sound/DSP/Resample.hpp>
#include <Sound/Jukebox.hpp>
#include <Sound/Listener.hpp>

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
        devices();
        for (const Device &device : _devices) {
            if (device.id == Pa_GetDefaultOutputDevice()) {
                set_output(device);
                break;
            }
        }
        for (const Device &device : _devices) {
            if (device.id != Pa_GetDefaultInputDevice() &&
                device.input_channels > 0) {
                set_input(device);
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

    void Jukebox::process_source(Source &source) {
        // Calculate the number of frames in the destination buffer
        Buffer &buffer = source._buffer;
        double frame_stop = std::min(source._frame + MAX_CHUNK_LENGTH,
                                     static_cast<double>(buffer.frames()));
        double frames = frame_stop - source._frame;

        // Calculate the number of frames required to process
        double factor = STANDARD_SAMPLE_RATE / _output_state.sample_rate;
        double length = frames * factor;

        // Resample to the device sample rate
        _scratch.resize(frames, buffer.channels());
        for (unsigned c = 0; c < buffer.channels(); c++) {
            resample_signal(buffer[c],
                            _scratch[c],
                            source._frame,
                            length,
                            STANDARD_SAMPLE_RATE,
                            _output_state.sample_rate);
        }

        // Apply the filters
        if (source._filter.has_value()) {
            Filter &filter = source._filter.value();
            filter.apply(_scratch, _scratch, source, _listener);
        }

        // Remix to the output device channels
        _remixed.resize(_scratch.frames(), _output_state.channels);
        _remixed.silence();
        _scratch.remix(_remixed);

        // Mix the processed sound onto the composite signal
        for (unsigned c = 0; c < _composite.channels(); c++) {
            Vectorize::vsma(_remixed[c],
                            _volume,
                            _composite[c],
                            _remixed.frames());
        }

        // Advance chunk frame
        source._frame += length;
    }

    Listener &Jukebox::listener() { return _listener; }

    const std::vector<Device> &Jukebox::devices() {
        PaError err;

        // Count the devices
        int device_count = Pa_GetDeviceCount();
        if (device_count < 0) {
            err = device_count;
            Log::error("PortAudio failed to count sound devices: {}",
                       Pa_GetErrorText(err));
        }

        // List all devices
        _devices.clear();
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

            _devices.emplace_back(device);
        }

        return _devices;
    }

    void Jukebox::set_input(const Device &device) {
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

    void Jukebox::set_output(const Device &device) {
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

    void Jukebox::set_volume(float volume) {
        _volume = std::clamp(volume, 0.0f, 1.0f);
    }

    float Jukebox::get_volume() const { return _volume; }

    bool Jukebox::is_playing() {
        return _output_stream != nullptr && Pa_IsStreamActive(_output_stream);
    }

    bool Jukebox::is_recording() {
        return _input_stream != nullptr && Pa_IsStreamActive(_input_stream);
    }

    void Jukebox::play(Source &source) {
        if (source._playing) return;

        source._playing = true;
        _sources.emplace_back(source);
    }

    void Jukebox::pause(Source &source) {
        if (!source._playing) return;

        auto d_it = _sources.begin();
        while (d_it != _sources.end()) {
            if (&d_it->get() == &source) {
                _sources.erase(d_it);
                source._playing = false;
                break;
            } else {
                d_it++;
            }
        }
    }

    void Jukebox::resume() { Pa_StartStream(_output_stream); }

    void Jukebox::pause() { Pa_StopStream(_output_stream); }

    void Jukebox::update() {
        // Wait for there to be available space in the buffer
        unsigned min_length = MAX_CHUNK_LENGTH * _output_state.channels;
        if (_output_state.buffer.remaining() < min_length || !is_playing()) {
            return;
        }

        // Zero-out the composite waveform
        _composite.silence();

        // Process chunks and mix onto the composite
        auto d_it = _sources.begin();
        while (d_it != _sources.end()) {
            Source &source = *d_it;
            if (source._frame >= source._frame_stop) {
                d_it = _sources.erase(d_it);
                source._playing = false;
                source._on_finish();
            } else {
                process_source(source);
                d_it++;
            }
        }

        // Clamp channels
        for (unsigned c = 0; c < _composite.channels(); c++) {
            Vectorize::vclamp(_composite[c],
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