#include "Messenger.hpp"

namespace Dynamo {
    void Messenger::log(std::string content) {
        const auto timestamp = std::chrono::system_clock::now();
        Message message = {timestamp, content, MessageType::Log};
        _log.push_back(message);
        std::cout << message.format() << std::endl;
    }

    void Messenger::error(std::string content) {
        const auto timestamp = std::chrono::system_clock::now();
        Message message = {timestamp, content, MessageType::Error};
        _log.push_back(message);
        std::cerr << message.format() << std::endl;

        // Terminate and write the log to disk
        dump();
        throw std::runtime_error("Dynamo has crashed.");
    }

    void Messenger::warn(std::string content) {
        const auto timestamp = std::chrono::system_clock::now();
        Message message = {timestamp, content, MessageType::Warning};
        _log.push_back(message);
        std::cout << message.format() << std::endl;
    }

    void Messenger::dump() {
        auto timestamp = std::chrono::floor<std::chrono::seconds>(
            std::chrono::system_clock::now());
        std::string filename =
            fmt::format("Dynamo_{:%Y-%m-%d_%H-%M-%S}.log", timestamp);

        std::ofstream outfile;
        outfile.open(filename, std::ios::out);

        for (Message &message : _log) {
            std::string formatted = message.format();
            outfile.write(formatted.c_str(), formatted.length());
            outfile.write("\n", 1);
        }
    }
} // namespace Dynamo