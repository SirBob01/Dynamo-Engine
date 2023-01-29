#pragma once

#include <string>

#include "../Types.hpp"

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
        i32 id;

        /**
         * @brief Name of the device
         *
         */
        std::string name;

        /**
         * @brief Number of available input channels
         *
         */
        u32 input_channels;

        /**
         * @brief Number of available output channels
         *
         */
        u32 output_channels;

        /**
         * @brief Compatible sample rate
         *
         */
        f32 sample_rate;
    };
} // namespace Dynamo::Sound