#pragma once
#ifndef NDEBUG
#define DYN_ASSERT(cond)                                                       \
    !(cond) ? Dynamo::Log::error("Assertion {} failed: {}, Line {}",           \
                                 #cond,                                        \
                                 __FILE__,                                     \
                                 __LINE__)                                     \
            : (void(0))
#else
#define DYN_ASSERT(cond) (void(0))
#endif

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
    class Log {
        inline static std::vector<Message> _history;

      public:
        /**
         * @brief Log an informative message to stdout
         *
         * @param format Formatting template string
         * @param args   Data arguments (optional)
         */
        template <typename... Types>
        static void info(std::string format = "", Types... args) {
            const auto timestamp = std::chrono::system_clock::now();
            std::string content = fmt::format(format, args...);
            Message message = {timestamp, content, MessageType::Info};
            _history.push_back(message);
            std::cout << message.format() << std::endl;
        }

        /**
         * @brief Log an error to stderr and terminate the process
         *
         * @param format Formatting template string
         * @param args   Data arguments (optional)
         */
        template <typename... Types>
        static void error(std::string format, Types... args) {
            const auto timestamp = std::chrono::system_clock::now();
            std::string content = fmt::format(format, args...);
            Message message = {timestamp, content, MessageType::Error};
            _history.push_back(message);
            std::cerr << message.format() << std::endl;

            // Terminate and write the log to disk
            dump();
            throw std::runtime_error("Dynamo has crashed.");
        }

        /**
         * @brief Log a warning to stderr
         *
         * @param format Formatting template string
         * @param args   Data arguments (optional)
         */
        template <typename... Types>
        static void warn(std::string format, Types... args) {
            const auto timestamp = std::chrono::system_clock::now();
            std::string content = fmt::format(format, args...);
            Message message = {timestamp, content, MessageType::Warning};
            _history.push_back(message);
            std::cerr << message.format() << std::endl;
        }

        /**
         * @brief Dump the log history to disk
         *
         * @param filename
         */
        static void dump();
    };
} // namespace Dynamo
