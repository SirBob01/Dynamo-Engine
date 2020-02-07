#include "error.h"

namespace Dynamo {
    const char *SDLError::what() const throw() {
        return SDL_GetError();
    }

    GenericError::GenericError(std::string message) {
        message_ = message;
    }

    const char *GenericError::what() const throw() {
        return message_.c_str();
    }

    PacketOverflow::PacketOverflow(int max, int size) {
        len_ = std::to_string(size);
        max_ = std::to_string(max);
        message_ = "Invalid packet size " + len_ + " (max " + max_ + ")";
    }

    const char *PacketOverflow::what() const throw() {
        return message_.c_str();
    }

    InvalidKey::InvalidKey(std::string key, std::string map_name) {
        message_ = key + " not found in " + map_name;
    }

    const char *InvalidKey::what() const throw() {
        return message_.c_str();
    }
}
