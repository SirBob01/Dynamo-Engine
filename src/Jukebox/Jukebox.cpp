#include "Jukebox.hpp"

namespace Dynamo {
    Jukebox::Jukebox(SoundManager &assets) : _assets(assets) {
        _volume = 1.0f;

        _playing = true;
        _recording = false;

        // Initialize state
        _state.input_channels = 0;
        _state.output_channels = 0;

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
                                   paInt16,
                                   SAMPLE_RATE,
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
        const float *device_input = static_cast<const float *>(input);
        float *device_output = static_cast<float *>(output);

        // Receive input from the device
        for (int f = 0; f < frame_count; f++) {
            for (int c = 0; c < state.input_channels; c++) {
                if (state.input_buffer.is_full()) {
                    state.input_buffer.clear();
                }
                state.input_buffer.write(
                    device_input[f * state.input_channels + c]);
            }
        }

        // Push the local buffer onto the device
        for (int f = 0; f < frame_count; f++) {
            for (int c = 0; c < state.output_channels; c++) {
                if (!state.output_buffer.is_empty()) {
                    device_output[f * state.output_channels + c] =
                        state.output_buffer.read();
                }
            }
        }
        return 0;
    }

    bool Jukebox::is_playing() { return _playing; }

    bool Jukebox::is_recording() { return _recording; }

    float Jukebox::get_volume() { return _volume; }

    void Jukebox::set_volume(float volume) {
        _volume = std::min(std::max(volume, 0.0f), 1.0f);
    }

    void Jukebox::play_static(Asset<Sound> &sound, float volume, float start) {
        Sound &data = _assets.get(sound);

        // Calculate starting frame
        int frame = static_cast<int>(std::floor(SAMPLE_RATE * start));

        // Get pointer position within the waveform
        int index = frame * data.channels;
        if (index >= data.waveform.size()) return;

        // TODO: Implement system for queuing audio to be processed and placed
        // onto the buffer
    }

    void Jukebox::pause() {
        Pa_StopStream(_stream);
        _playing = false;
    }

    void Jukebox::resume() {
        Pa_StartStream(_stream);
        _playing = true;
    }
} // namespace Dynamo