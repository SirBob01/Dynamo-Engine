#pragma once

#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "Message.hpp"

namespace Dynamo {
    /**
     * @brief Logging utility handler
     *
     */
    class Messenger {
        inline static std::vector<Message> _log;

      public:
        /**
         * @brief Log a message to stdout
         *
         * @param content Message to be displayed
         */
        static void log(std::string content);

        /**
         * @brief Log an error to stderr and terminate the process
         *
         * @param content Message to be displayed
         */
        static void error(std::string content);

        /**
         * @brief Log a warning to stderr
         *
         * @param content Message to be displayed
         */
        static void warn(std::string content);

        /**
         * @brief Dump the log history to disk
         *
         * @param filename
         */
        static void dump();
    };
} // namespace Dynamo
