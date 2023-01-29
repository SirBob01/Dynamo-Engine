#pragma once

#include <algorithm>
#include <vector>

#include "../Types.hpp"
#include "../Log/Log.hpp"

namespace Dynamo {
    /**
     * @brief Dynamic multi-channel container where data is stored in a
     * deinterleaved layout
     *
     * @tparam T Type of data
     */
    template <typename T>
    class ChannelData {
        static_assert(std::is_trivially_copyable<T>::value,
                      "ChannelData members must be trivially copyable");
        /**
         * @brief Data container
         *
         */
        std::vector<T> _container;

        /**
         * @brief Number of channels
         *
         */
        u32 _channels;

        /**
         * @brief Number of frames
         *
         */
        u32 _frames;

      public:
        /**
         * @brief Construct an empty ChannelData object
         *
         * @param frames   Number of frames
         * @param channels Number of channels
         */
        ChannelData(u32 frames = 0, u32 channels = 0) {
            resize(frames, channels);
        }

        /**
         * @brief Construct a ChannelData object with an existing buffer
         *
         * @param data     Data buffer
         * @param channels Number of channels
         */
        ChannelData(std::vector<T> data, u32 channels) :
            _container(data), _channels(channels),
            _frames(data.size() / channels) {
            DYN_ASSERT(data.size() % channels == 0);
        }

        /**
         * @brief Get internal data pointer
         *
         * @return T*
         */
        inline T *data() { return _container.data(); }

        /**
         * @brief Get the number of frames
         *
         * @return u32
         */
        inline u32 frames() const { return _frames; }

        /**
         * @brief Get the number of channels
         *
         * @return u32
         */
        inline u32 channels() const { return _channels; }

        /**
         * @brief Default-initialize all the data
         *
         */
        inline void clear() {
            std::fill(_container.begin(), _container.end(), T());
        }

        /**
         * @brief Set the number of frames
         *
         * This will resize the container
         *
         * @param frames Number of frames
         */
        inline void set_frames(const u32 frames) {
            _container.resize(_channels * frames);
            _frames = frames;
        }

        /**
         * @brief Set the number of channels
         *
         * This will resize the container
         *
         * @param channels Number of channels
         */
        inline void set_channels(const u32 channels) {
            _container.resize(channels * _frames);
            _channels = channels;
        }

        /**
         * @brief Resize the container
         *
         * @param frames   Number of frames
         * @param channels Number of channels
         */
        inline void resize(const u32 frames, const u32 channels) {
            _container.resize(channels * frames);
            _channels = channels;
            _frames = frames;
        }

        /**
         * @brief Read a frame into a buffer
         *
         * @param dst   Destination buffer
         * @param frame Frame index
         */
        inline void read_frame(T *dst, const u32 frame) const {
            DYN_ASSERT(frame < _frames);
            for (u32 c = 0; c < _channels; c++) {
                dst[c] = _container[(c * _frames) + frame];
            }
        }

        /**
         * @brief Read a channel into a buffer
         *
         * @param dst     Destination buffer
         * @param channel Channel index
         */
        inline void read_channel(T *dst, const u32 channel) const {
            DYN_ASSERT(channel < _channels);
            const T *ptr = _container.data() + (channel * _frames);
            std::copy(ptr, ptr + _frames, dst);
        }

        /**
         * @brief Get the pointer to the start of a channel
         *
         * @param channel
         * @return T*
         */
        inline T *operator[](const u32 channel) {
            DYN_ASSERT(channel < _channels);
            return _container.data() + (channel * _frames);
        }

        /**
         * @brief Get a value in the container
         *
         * @param frame   Frame index
         * @param channel Channel index
         * @return T
         */
        inline T at(const u32 frame, const u32 channel) const {
            DYN_ASSERT(frame < _frames);
            DYN_ASSERT(channel < _channels);
            return _container[(channel * _frames) + frame];
        }
    };
} // namespace Dynamo