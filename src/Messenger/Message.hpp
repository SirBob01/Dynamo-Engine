#pragma once

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace Dynamo {
    /**
     * @brief Message object
     *
     */
    struct Message {
        std::chrono::_V2::system_clock::time_point timestamp;
        std::string content;
        bool is_error;

        /**
         * @brief Get the string representation of the message
         *
         * @return std::string
         */
        std::string format();
    };
} // namespace Dynamo
