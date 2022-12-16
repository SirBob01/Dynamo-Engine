#pragma once

#include <algorithm>
#include <vector>

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
        std::vector<T> _data;

        /**
         * @brief Number of channels
         *
         */
        unsigned _channels;

        /**
         * @brief Number of frames
         *
         */
        unsigned _frames;

      public:
        /**
         * @brief Construct an empty ChannelData object
         *
         * @param channels Number of channels
         * @param frames   Number of frames
         */
        ChannelData(unsigned channels = 0, unsigned frames = 0) :
            _channels(channels), _frames(frames) {
            resize(frames, channels);
        }

        /**
         * @brief Construct a ChannelData object with an existing buffer
         *
         * @param data     Data buffer
         * @param channels Number of channels
         */
        ChannelData(std::vector<T> data, unsigned channels) :
            _data(data), _channels(channels), _frames(data.size() / channels) {
            DYN_ASSERT(data.size() % channels == 0);
        }

        /**
         * @brief Get the internal data container
         *
         * @return std::vector<T>&
         */
        inline std::vector<T> &data() { return _data; }

        /**
         * @brief Get the number of frames
         *
         * @return unsigned
         */
        inline unsigned frames() const { return _frames; }

        /**
         * @brief Get the number of channels
         *
         * @return unsigned
         */
        inline unsigned channels() const { return _channels; }

        /**
         * @brief Default-initialize all the data
         *
         */
        inline void clear() { std::fill(_data.begin(), _data.end(), T()); }

        /**
         * @brief Set the number of frames
         *
         * This will resize the container
         *
         * @param frames Number of frames
         */
        inline void set_frames(const unsigned frames) {
            _data.resize(_channels * frames);
            _frames = frames;
        }

        /**
         * @brief Set the number of channels
         *
         * This will resize the container
         *
         * @param channels Number of channels
         */
        inline void set_channels(const unsigned channels) {
            _data.resize(channels * _frames);
            _channels = channels;
        }

        /**
         * @brief Resize the container
         *
         * @param frames   Number of frames
         * @param channels Number of channels
         */
        inline void resize(const unsigned frames, const unsigned channels) {
            _data.resize(channels * frames);
            _channels = channels;
            _frames = frames;
        }

        /**
         * @brief Read a frame into a buffer
         *
         * @param dst   Destination buffer
         * @param frame Frame index
         */
        inline void read_frame(T *dst, const unsigned frame) const {
            DYN_ASSERT(frame < _frames);
            for (unsigned c = 0; c < _channels; c++) {
                dst[c] = _data[(c * _frames) + frame];
            }
        }

        /**
         * @brief Read a channel into a buffer
         *
         * @param dst     Destination buffer
         * @param channel Channel index
         */
        inline void read_channel(T *dst, const unsigned channel) const {
            DYN_ASSERT(channel < _channels);
            const T *ptr = _data.data() + (channel * _frames);
            std::copy(ptr, ptr + _frames, dst);
        }

        /**
         * @brief Get the pointer to the start of a channel
         *
         * @param channel
         * @return T*
         */
        inline T *operator[](const unsigned channel) {
            DYN_ASSERT(channel < _channels);
            return _data.data() + (channel * _frames);
        }

        /**
         * @brief Get a value in the container
         *
         * @param frame   Frame index
         * @param channel Channel index
         * @return T
         */
        inline T at(const unsigned frame, const unsigned channel) const {
            DYN_ASSERT(frame < _frames && channel < _channels);
            return _data[(channel * _frames) + frame];
        }
    };
} // namespace Dynamo