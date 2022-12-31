#pragma once

#include <string>

namespace Dynamo::Sound {
    /**
     * @brief Device for reading and/or writing audio data
     *
     */
    struct Device {
        /**
         * @brief Unique identifier of the device
         *
         */
        int id;

        /**
         * @brief Name of the device
         *
         */
        std::string name;

        /**
         * @brief Number of available input channels
         *
         */
        unsigned input_channels;

        /**
         * @brief Number of available output channels
         *
         */
        unsigned output_channels;

        /**
         * @brief Compatible sample rate
         *
         */
        float sample_rate;
    };
} // namespace Dynamo::Sound