#include "Message.hpp"

namespace Dynamo {
    std::string Message::format() {
        switch (type) {
        case MessageType::Error:
            return fmt::format("[ERROR {:%Y-%m-%d %H:%M:%S}] {}",
                               timestamp,
                               content);
            break;
        case MessageType::Warning:
            return fmt::format("[WARN {:%Y-%m-%d %H:%M:%S}] {}",
                               timestamp,
                               content);
            break;
        default:
            return fmt::format("[DEBUG {:%Y-%m-%d %H:%M:%S}] {}",
                               timestamp,
                               content);
            break;
        }
    }
} // namespace Dynamo