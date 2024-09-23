#pragma once

#include <algorithm>
#include <vector>

#include "./Log.hpp"

namespace Dynamo {
    /**
     * @brief Dynamic multi-channel container where data is stored in a
     * deinterleaved layout.
     *
     * @tparam T Type of data.
     */
    template <typename T>
    class ChannelData {
        static_assert(std::is_trivially_copyable<T>::value,
                      "ChannelData members must be trivially copyable");
        std::vector<T> _container;

        unsigned _channels;
        unsigned _frames;

      public:
        /**
         * @brief Construct an empty ChannelData object.
         *
         * @param frames   Number of frames.
         * @param channels Number of channels.
         */
        ChannelData(unsigned frames = 0, unsigned channels = 0) {
            resize(frames, channels);
        }

        /**
         * @brief Construct a ChannelData object with an existing buffer.
         *
         * @param data     Data buffer.
         * @param channels Number of channels.
         */
        ChannelData(std::vector<T> data, unsigned channels) :
            _container(data), _channels(channels),
            _frames(data.size() / channels) {
            DYN_ASSERT(data.size() % channels == 0);
        }

        /**
         * @brief Get internal data pointer.
         *
         * @return T*
         */
        inline T *data() { return _container.data(); }

        /**
         * @brief Get the number of frames.
         *
         * @return unsigned
         */
        inline unsigned frames() const { return _frames; }

        /**
         * @brief Get the number of channels.
         *
         * @return unsigned
         */
        inline unsigned channels() const { return _channels; }

        /**
         * @brief Default-initialize all entries in the container.
         *
         */
        inline void clear() {
            std::fill(_container.begin(), _container.end(), T());
        }

        /**
         * @brief Set the number of frames and resize the container.
         *
         * @param frames Number of frames.
         */
        inline void set_frames(const unsigned frames) {
            _container.resize(_channels * frames);
            _frames = frames;
        }

        /**
         * @brief Set the number of channels and resize the container.
         *
         * @param channels Number of channels.
         */
        inline void set_channels(const unsigned channels) {
            _container.resize(channels * _frames);
            _channels = channels;
        }

        /**
         * @brief Resize the container to fit a number of frames and channels.
         *
         * @param frames   Number of frames.
         * @param channels Number of channels.
         */
        inline void resize(const unsigned frames, const unsigned channels) {
            _container.resize(channels * frames);
            _channels = channels;
            _frames = frames;
        }

        /**
         * @brief Read a frame into a buffer.
         *
         * @param dst   Destination buffer.
         * @param frame Frame index.
         */
        inline void read_frame(T *dst, const unsigned frame) const {
            DYN_ASSERT(frame < _frames);
            for (unsigned c = 0; c < _channels; c++) {
                dst[c] = _container[(c * _frames) + frame];
            }
        }

        /**
         * @brief Read a channel into a buffer.
         *
         * @param dst     Destination buffer.
         * @param channel Channel index.
         */
        inline void read_channel(T *dst, const unsigned channel) const {
            DYN_ASSERT(channel < _channels);
            const T *ptr = _container.data() + (channel * _frames);
            std::copy(ptr, ptr + _frames, dst);
        }

        /**
         * @brief Get the pointer to the start of a channel.
         *
         * @param channel Channel index.
         * @return T*
         */
        inline T *operator[](const unsigned channel) {
            DYN_ASSERT(channel < _channels);
            return _container.data() + (channel * _frames);
        }

        /**
         * @brief Get a value in the container.
         *
         * @param frame   Frame index.
         * @param channel Channel index.
         * @return T
         */
        inline T at(const unsigned frame, const unsigned channel) const {
            DYN_ASSERT(frame < _frames);
            DYN_ASSERT(channel < _channels);
            return _container[(channel * _frames) + frame];
        }
    };
} // namespace Dynamo