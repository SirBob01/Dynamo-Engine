#pragma once

#include <chrono>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

namespace Dynamo {
    /**
     * @brief Enumerates the different warning types.
     *
     */
    enum class MessageType { Info, Warning, Error };

    /**
     * @brief Message object.
     *
     */
    struct Message {
        std::chrono::system_clock::time_point timestamp;
        std::string content;
        MessageType type;

        /**
         * @brief Get the string representation of the message.
         *
         * @return std::string
         */
        std::string format();
    };
} // namespace Dynamo
