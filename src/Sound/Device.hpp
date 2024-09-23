#pragma once

#include <string>

namespace Dynamo::Sound {
    /**
     * @brief Audio physical device.
     *
     */
    struct Device {
        /**
         * @brief Unique identifier of the device.
         *
         */
        int id;

        /**
         * @brief Name of the device.
         *
         */
        std::string name;

        /**
         * @brief Number of available input channels.
         *
         */
        unsigned input_channels;

        /**
         * @brief Number of available output channels.
         *
         */
        unsigned output_channels;

        /**
         * @brief Compatible sample rate.
         *
         */
        float sample_rate;

        /**
         * @brief Input latency.
         *
         */
        float input_latency;

        /**
         * @brief Output latency.
         *
         */
        float output_latency;
    };
} // namespace Dynamo::Sound