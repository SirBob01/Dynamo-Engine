#include "Jukebox.hpp"

namespace Dynamo {
    Jukebox::Jukebox() {
        // Initialize state
        _state.input_channels = 0;
        _state.output_channels = 0;
        _state.volume = 1.0f;
        _state.playing = true;
        _state.recording = false;

        // Initialize PortAudio
        PaError err;
        err = Pa_Initialize();
        if (err != paNoError) {
            Log::error("Could not initialize PortAudio subsystem: {}",
                       Pa_GetErrorText(err));
        }

        // Get list of sound devices
        int device_count = Pa_GetDeviceCount();
        if (device_count < 0) {
            err = device_count;
            Log::error("PortAudio failed to count sound devices: {}",
                       Pa_GetErrorText(err));
        }

        // Calculate default input and output channels
        for (int index = 0; index < device_count; index++) {
            const PaDeviceInfo *device_info = Pa_GetDeviceInfo(index);
            if (index == Pa_GetDefaultInputDevice()) {
                _state.input_channels = device_info->maxInputChannels;
            } else if (index == Pa_GetDefaultOutputDevice()) {
                _state.output_channels = device_info->maxOutputChannels;
            }
        }
        if (!_state.output_channels) {
            Log::error("PortAudio could not find suitable audio devices.");
        }

        // Start the IO stream
        err = Pa_OpenDefaultStream(&_stream,
                                   _state.input_channels,
                                   _state.output_channels,
                                   paFloat32,
                                   JUKEBOX_SAMPLE_RATE,
                                   paFramesPerBufferUnspecified,
                                   stream_callback,
                                   &_state);
        if (err != paNoError) {
            Log::error("Could not open PortAudio stream: {}",
                       Pa_GetErrorText(err));
        }

        err = Pa_StartStream(_stream);
        if (err != paNoError) {
            Log::error("Could not start PortAudio stream: {}",
                       Pa_GetErrorText(err));
        }
    }

    Jukebox::~Jukebox() {
        PaError err;

        // Close the IO stream
        err = Pa_CloseStream(_stream);
        if (err != paNoError) {
            Log::error("PortAudio could not close stream: {}",
                       Pa_GetErrorText(err));
        }

        // Uninitialize PortAudio
        err = Pa_Terminate();
        if (err != paNoError) {
            Log::error("Could not terminate PortAudio subsystem: {}",
                       Pa_GetErrorText(err));
        }
    }

    int Jukebox::stream_callback(const void *input,
                                 void *output,
                                 unsigned long frame_count,
                                 const PaStreamCallbackTimeInfo *time_info,
                                 PaStreamCallbackFlags status_flags,
                                 void *data) {
        State &state = *static_cast<State *>(data);
        const float *input_buffer = static_cast<const float *>(input);
        float *output_buffer = static_cast<float *>(output);
        return 0;
    }

    bool Jukebox::is_playing() { return _state.playing; }

    bool Jukebox::is_recording() { return _state.recording; }

    float Jukebox::get_volume() { return _state.volume; }

    void Jukebox::set_volume(float volume) {
        _state.volume = std::min(std::max(volume, 0.0f), 1.0f);
    }
} // namespace Dynamo