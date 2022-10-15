#ifndef DYNAMO_MESSENGER_HPP_
#define DYNAMO_MESSENGER_HPP_

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "./Message.hpp"

namespace Dynamo {
    /**
     * @brief Enumerates all possible points of failure in the engine
     *
     */
    enum ErrorCode { CoreFailure };

    /**
     * @brief Messages associated with each error code
     *
     */
    static const std::unordered_map<ErrorCode, std::string> ErrorMessages = {
        {ErrorCode::CoreFailure, "An internal failure has occurred."}};

    /**
     * @brief Logging utility handler
     *
     */
    class Messenger {
        /**
         * @brief List of all logged messages
         *
         */
        inline static std::vector<Message> _log;

      public:
        /**
         * @brief Log a message to stdout
         *
         * @param content Message to be displayed
         */
        static void log(std::string content);

        /**
         * @brief Log an error to stderr
         *
         * @param code Error code
         * @param exit Terminate the process
         */
        static void error(ErrorCode code, bool exit = true);

        /**
         * @brief Dump the log history to disk
         *
         * @param filename
         */
        static void dump();
    };
} // namespace Dynamo

#endif