#ifndef DYNAMO_MESSENGER_MESSAGE_HPP_
#define DYNAMO_MESSENGER_MESSAGE_HPP_

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
        /**
         * @brief Exact time the message was generated
         *
         */
        std::chrono::_V2::system_clock::time_point timestamp;

        /**
         * @brief Message content
         *
         */
        std::string content;

        /**
         * @brief Is the message an error?
         *
         */
        bool error;

        /**
         * @brief Get the string representation of the message
         *
         * @return std::string
         */
        std::string format();
    };
} // namespace Dynamo

#endif