#pragma once

#include <vector>

#include <portaudio.h>

#include <Utils/IdTracker.hpp>
#include <Utils/RingBuffer.hpp>

#include <Sound/Chunk.hpp>
#include <Sound/Device.hpp>
#include <Sound/Listener.hpp>
#include <Sound/Material.hpp>
#include <Sound/Sound.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Size of the input and output ring buffers
     *
     */
    static constexpr unsigned BUFFER_SIZE = MAX_CHUNK_LENGTH * 64;

    /**
     * @brief In-house audio engine supporting static sounds, spatial sounds,
     * and multiple listeners
     *
     */
    class Jukebox {
        PaStream *_input_stream;
        PaStream *_output_stream;

        float _volume;

        Sound _scratch;
        Sound _remixed;
        Sound _composite;

        std::vector<ListenerRef> _listeners;
        std::vector<Chunk> _chunks;

        /**
         * @brief Internal state shared with the PortAudio callback
         *
         */
        struct PaState {
            RingBuffer<WaveSample, BUFFER_SIZE> buffer;
            unsigned channels;
            double sample_rate;
        };
        PaState _input_state;
        PaState _output_state;

        /**
         * @brief Callback for reading recorded audio from the input device
         *
         * @param input        Input buffer
         * @param output       Output buffer
         * @param frame_count  Frames to read
         * @param time_info    Time information in seconds
         * @param status_flags Status flags
         * @param data         User data
         * @return int
         */
        static int input_callback(const void *input,
                                  void *output,
                                  unsigned long frame_count,
                                  const PaStreamCallbackTimeInfo *time_info,
                                  PaStreamCallbackFlags status_flags,
                                  void *data);

        /**
         * @brief Callback for pushing audio data to the output device
         *
         * @param input        Input buffer
         * @param output       Output buffer
         * @param frame_count  Frames to write
         * @param time_info    Time information in seconds
         * @param status_flags Status flags
         * @param data         User data
         * @return int
         */
        static int output_callback(const void *input,
                                   void *output,
                                   unsigned long frame_count,
                                   const PaStreamCallbackTimeInfo *time_info,
                                   PaStreamCallbackFlags status_flags,
                                   void *data);

        /**
         * @brief Find the closest listener to a position
         *
         * @param position
         * @return Listener&
         */
        Listener &find_closest_listener(Vec3 position);

        /**
         * @brief Process a chunk
         *
         * @param chunk chunk
         */
        void process_chunk(Chunk &chunk);

      public:
        /**
         * @brief Construct a new Jukebox object
         *
         */
        Jukebox();
        ~Jukebox();

        /**
         * @brief Get all available sound devices
         *
         * @return const std::vector<Device>
         */
        const std::vector<Device> get_devices();

        /**
         * @brief Add a listener.
         *
         * Jukebox requires at least 1 listener for playback.
         *
         * @param listener
         */
        void add_listener(Listener &listener);

        /**
         * @brief Remove a listener.
         *
         * Returns true if the listener was found.
         *
         * @param listener
         * @return true
         * @return false
         */
        bool remove_listener(Listener &listener);

        /**
         * @brief Remove all listeners.
         *
         */
        void clear_listeners();

        /**
         * @brief Set the input device.
         *
         * This will close the input stream and instance a new one.
         *
         * @param device
         */
        void set_input_device(const Device &device);

        /**
         * @brief Set the output device.
         *
         * This will close the output stream and instance a new one.
         *
         * @param device
         */
        void set_output_device(const Device &device);

        /**
         * @brief Is the output device playing?
         *
         * @return true
         * @return false
         */
        bool is_playing();

        /**
         * @brief Is the input device listening?
         *
         * @return true
         * @return false
         */
        bool is_recording();

        /**
         * @brief Get the master volume.
         *
         * @return float
         */
        float get_volume();

        /**
         * @brief Set the master volume.
         *
         * @param volume
         */
        void set_volume(float volume);

        /**
         * @brief Play a sound.
         *
         * These sounds are spatialized and affected by the position of the
         * sound and the listeners. If there are multiple listeners attached,
         * the sound will be processed for all of them and mixed.
         *
         * @param sound    Sound
         * @param material Playback properties
         * @param filters  Filter
         */
        void play(Sound &sound,
                  Material &material,
                  std::optional<FilterRef> filter = {});

        /**
         * @brief Pause audio playback.
         *
         */
        void pause();

        /**
         * @brief Resume audio playback.
         *
         */
        void resume();

        /**
         * @brief Update Jukebox's internal state and process all chunks
         * to be written into the output buffer.
         *
         */
        void update();
    };
} // namespace Dynamo::Sound