#pragma once

#include <algorithm>
#include <vector>

#include "../Log/Log.hpp"

namespace Dynamo {
    /**
     * @brief String of data allocated into multiple channels
     *
     * @tparam T Type of data
     */
    template <typename T>
    class ChannelData {
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
            resize(frames);
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
         * @return int
         */
        inline int frames() { return _frames; }

        /**
         * @brief Get the number of channels
         *
         * @return int
         */
        inline int channels() { return _channels; }

        /**
         * @brief Default-initialize all the data
         *
         */
        inline void clear() { std::fill(_data.begin(), _data.end(), T()); }

        /**
         * @brief Set the number of channels
         *
         * This will resize the container
         *
         * @param channels Number of channels
         */
        inline void set_channels(int channels) {
            _data.resize(channels * _frames);
            _channels = channels;
        }

        /**
         * @brief Resize the container to fit a certain number of frames
         *
         * @param frames Number of frames to contain
         */
        inline void resize(int frames) {
            _data.resize(_channels * frames);
            _frames = frames;
        }

        /**
         * @brief Get a reference to a value in the container
         *
         * @param frame   Frame index [0, |data| / channels]
         * @param channel Channel index [0, channels - 1]
         * @return T&
         */
        inline T &at(unsigned frame, unsigned channel) {
            // Struct of arrays access pattern for better cache locality
            DYN_ASSERT(frame < _frames && channel < _channels);
            int index = (channel * _frames) + frame;
            return _data[index];
        }

        /**
         * @brief Get a reference to a value in the container using an
         * interleaved access pattern
         *
         * @param frame   Frame index [0, |data| / channels]
         * @param channel Channel index [0, channels - 1]
         * @return T&
         */
        inline T &at_interleaved(unsigned frame, unsigned channel) {
            // Array of structs access pattern
            DYN_ASSERT(frame < _frames && channel < _channels);
            int index = (frame * _channels) + channel;
            return _data[index];
        }
    };
} // namespace Dynamo