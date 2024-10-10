#pragma once

#ifndef NDEBUG
#define DYN_DEBUG
#endif

#ifdef DYN_DEBUG
#define DYN_ASSERT(cond)                                                                                               \
    !(cond) ? Dynamo::Log::error("Assertion {} failed: {}, Line {}", #cond, __FILE__, __LINE__) : (void(0))
#else
#define DYN_ASSERT(cond) (void(0))
#endif

#include <iostream>
#include <string>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <Clock.hpp>

namespace Dynamo {
    /**
     * @brief Log helper.
     *
     */
    class Log {
      public:
        /**
         * @brief Log an information string.
         *
         * @tparam Args
         * @param fmt
         * @param args
         */
        template <typename... Args>
        static void info(std::string fmt = "", Args... args) {
            std::string message = fmt::format(fmt, args...);
            std::string line = fmt::format("[INFO {:%Y-%m-%d %H:%M:%S}] {}", Clock::time(), message);
            std::cout << line << "\n";
        }

        /**
         * @brief Log a warning.
         *
         * @tparam Args
         * @param fmt
         * @param args
         */
        template <typename... Args>
        static void warn(std::string fmt, Args... args) {
            std::string message = fmt::format(fmt, args...);
            std::string line = fmt::format("[WARN {:%Y-%m-%d %H:%M:%S}] {}", Clock::time(), message);
            std::cout << line << "\n";
        }

        /**
         * @brief Log an error. This will throw an exception.
         *
         * @tparam Args
         * @param fmt
         * @param args
         */
        template <typename... Args>
        static void error(std::string fmt, Args... args) {
            std::string message = fmt::format(fmt, args...);
            std::string line = fmt::format("[ERROR {:%Y-%m-%d %H:%M:%S}] {}", Clock::time(), message);
            std::cout << line << "\n";
            throw std::runtime_error("Dynamo fatal.");
        }
    };
} // namespace Dynamo