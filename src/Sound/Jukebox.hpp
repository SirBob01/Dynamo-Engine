#pragma once

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include <portaudio.h>

#include "../Asset/Asset.hpp"
#include "../Log/Log.hpp"
#include "../Math/Vec3.hpp"
#include "../Types.hpp"
#include "../Utils/IdTracker.hpp"
#include "../Utils/RingBuffer.hpp"
#include "../Utils/SparseSet.hpp"

#include "./Chunk.hpp"
#include "./Device.hpp"
#include "./Filters/Binaural.hpp"
#include "./Filters/Filter.hpp"
#include "./HRTF.hpp"
#include "./Listener.hpp"
#include "./Material.hpp"
#include "./Sound.hpp"
#include "./SoundCache.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Size of the input and output ring buffers
     *
     */
    static constexpr u32 BUFFER_SIZE = MAX_CHUNK_LENGTH * 64;

    /**
     * @brief A playback track on which audio can be enqueued
     *
     * TODO: Implement API
     *
     */
    using SoundTrack = Id;

    /**
     * @brief In-house audio engine supporting static sounds, spatial sounds,
     * and multiple listeners
     *
     */
    class Jukebox {
        SoundCache _assets;
        PaStream *_input_stream;
        PaStream *_output_stream;

        /**
         * @brief Global volume control
         *
         */
        f32 _volume;

        /**
         * @brief Final waveform on which all active chunks are layered onto
         *
         */
        WaveForm _composite;

        /**
         * @brief Set of all listeners
         *
         */
        ListenerSet _listeners;

        /**
         * @brief Static chunk list
         *
         */
        std::vector<Chunk<StaticMaterial>> _static_chunks;

        /**
         * @brief Dynamic chunk list
         *
         */
        std::vector<Chunk<DynamicMaterial>> _dynamic_chunks;

        /**
         * @brief Internal state shared with the PortAudio callback
         *
         */
        struct State {
            /**
             * @brief Buffer to read and write samples from asynchronously
             *
             */
            RingBuffer<WaveSample, BUFFER_SIZE> buffer;

            /**
             * @brief Number of channels in the buffer
             *
             */
            u32 channels;

            /**
             * @brief Sampling rate of the buffer
             *
             */
            f64 sample_rate;
        };
        State _input_state;
        State _output_state;

        /**
         * @brief Callback for reading recorded audio from the input device
         *
         * @param input        Input buffer
         * @param output       Output buffer
         * @param frame_count  Frames to read
         * @param time_info    Time information in seconds
         * @param status_flags Status flags
         * @param data         User data
         * @return i32
         */
        static i32 input_callback(const void *input,
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
         * @return i32
         */
        static i32 output_callback(const void *input,
                                   void *output,
                                   unsigned long frame_count,
                                   const PaStreamCallbackTimeInfo *time_info,
                                   PaStreamCallbackFlags status_flags,
                                   void *data);

        /**
         * @brief Process a static chunk
         *
         * @param chunk Static chunk
         */
        void process_chunk(Chunk<StaticMaterial> &chunk);

        /**
         * @brief Process a dynamic chunk
         *
         * @param chunk Dynamic chunk
         */
        void process_chunk(Chunk<DynamicMaterial> &chunk);

        /**
         * @brief Find the closest listener to a sound
         *
         * @param material
         * @return ListenerProperties&
         */
        ListenerProperties &
        find_closest_listener(const DynamicMaterial &material);

      public:
        /**
         * @brief Construct a new Jukebox object
         *
         * @param asset_directory Root asset directory
         */
        Jukebox(const std::string asset_directory);

        /**
         * @brief Destroy the Jukebox object
         *
         */
        ~Jukebox();

        /**
         * @brief Get all available sound devices
         *
         * @return const std::vector<Device>
         */
        const std::vector<Device> get_devices();

        /**
         * @brief Set the input device
         *
         * This will close the input stream and instance a new one
         *
         * @param device
         */
        void set_input_device(const Device &device);

        /**
         * @brief Set the output device
         *
         * This will close the output stream and instance a new one
         *
         * @param device
         */
        void set_output_device(const Device &device);

        /**
         * @brief Get the sample rate of the input stream
         *
         * @return u32
         */
        u32 get_input_sample_rate();

        /**
         * @brief Get the sample rate of the output stream
         *
         * @return u32
         */
        u32 get_output_sample_rate();

        /**
         * @brief Is the output device playing?
         *
         * @return true
         * @return false
         */
        b8 is_playing();

        /**
         * @brief Is the input device listening
         *
         * @return true
         * @return false
         */
        b8 is_recording();

        /**
         * @brief Get the master volume
         *
         * @return f32
         */
        f32 get_volume();

        /**
         * @brief Pause audio playback
         *
         */
        void pause();

        /**
         * @brief Resume audio playback
         *
         */
        void resume();

        /**
         * @brief Set the master volume
         *
         * @param volume
         */
        void set_volume(f32 volume);

        /**
         * @brief Get the sound asset cache
         *
         * @return SoundCache&
         */
        SoundCache &get_sounds();

        /**
         * @brief Get the set of listeners
         *
         * @return ListenerSet&
         */
        ListenerSet &get_listeners();

        /**
         * @brief Play a static sound
         *
         * @param sound    Sound asset
         * @param material Playback properties
         */
        void play(Asset<Sound> &sound, StaticMaterial &material);

        /**
         * @brief Play a dynamic sound
         *
         * These sounds are spatialized and affected by the position of the
         * sound and the listeners. If there are multiple listeners attached,
         * the sound will be processed for all of them and mixed.
         *
         * @param sound    Sound asset
         * @param material Playback properties
         */
        void play(Asset<Sound> &sound, DynamicMaterial &material);

        /**
         * @brief Update Jukebox's internal state and process all chunks
         * to be written into the output buffer
         *
         */
        void update();
    };
} // namespace Dynamo::Sound