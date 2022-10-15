#include "./Message.hpp"

namespace Dynamo {
    std::string Message::format() {
        if (error) {
            return fmt::format("[ERROR {:%Y-%m-%d %H:%M:%S}] {}",
                               timestamp,
                               content);
        }
        return fmt::format("[DEBUG {:%Y-%m-%d %H:%M:%S}] {}",
                           timestamp,
                           content);
    }
} // namespace Dynamo