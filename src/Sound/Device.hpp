#pragma once

#include <string>

#include "../Types.hpp"

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
        i32 id;

        /**
         * @brief Name of the device.
         *
         */
        std::string name;

        /**
         * @brief Number of available input channels.
         *
         */
        u32 input_channels;

        /**
         * @brief Number of available output channels.
         *
         */
        u32 output_channels;

        /**
         * @brief Compatible sample rate.
         *
         */
        f32 sample_rate;

        /**
         * @brief Input latency.
         *
         */
        f32 input_latency;

        /**
         * @brief Output latency.
         *
         */
        f32 output_latency;
    };
} // namespace Dynamo::Sound