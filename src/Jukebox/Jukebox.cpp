#include "Jukebox.hpp"

namespace Dynamo {
    Jukebox::Jukebox() {
        PaError err;

        // Initialize PortAudio
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
        int input_channels = 0;
        int output_channels = 0;
        for (int index = 0; index < device_count; index++) {
            const PaDeviceInfo *device_info = Pa_GetDeviceInfo(index);
            if (index == Pa_GetDefaultInputDevice()) {
                input_channels = device_info->maxInputChannels;
            } else if (index == Pa_GetDefaultOutputDevice()) {
                output_channels = device_info->maxOutputChannels;
            }
        }
        if (!output_channels) {
            Log::error("PortAudio could not find suitable audio devices.");
        }

        // Start the IO stream
        err = Pa_OpenDefaultStream(&_stream,
                                   input_channels,
                                   output_channels,
                                   paFloat32,
                                   JUKEBOX_SAMPLE_RATE,
                                   paFramesPerBufferUnspecified,
                                   stream_callback,
                                   nullptr // TODO: Pass userdata
        );
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
        return 0;
    }
} // namespace Dynamo