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

        err = Pa_OpenDefaultStream(&_stream,
                                   2, // Number of input channels
                                   2, // Number of output channels
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
        // TODO: Implement
        return 0;
    }
} // namespace Dynamo