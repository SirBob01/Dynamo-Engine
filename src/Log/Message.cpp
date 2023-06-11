#include "Message.hpp"

namespace Dynamo {
    std::string Message::format() {
        switch (type) {
        case MessageType::Error:
            return std::vformat("[ERROR {:%Y-%m-%d %H:%M:%S}] {}",
                                std::make_format_args(timestamp, content));
            break;
        case MessageType::Warning:
            return std::vformat("[WARN {:%Y-%m-%d %H:%M:%S}] {}",
                                std::make_format_args(timestamp, content));
            break;
        default:
            return std::vformat("[INFO {:%Y-%m-%d %H:%M:%S}] {}",
                                std::make_format_args(timestamp, content));
            break;
        }
    }
} // namespace Dynamo