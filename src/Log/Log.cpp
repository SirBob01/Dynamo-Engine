#include "Log.hpp"

namespace Dynamo {
    void Log::dump() {
        auto timestamp = std::chrono::floor<std::chrono::seconds>(
            std::chrono::system_clock::now());
        std::string filename =
            fmt::format("Dynamo_{:%Y-%m-%d_%H-%M-%S}.log", timestamp);

        std::ofstream outfile;
        outfile.open(filename, std::ios::out);

        for (Message &message : _history) {
            std::string formatted = message.format();
            outfile.write(formatted.c_str(), formatted.length());
            outfile.write("\n", 1);
        }
    }
} // namespace Dynamo