#include "error.h"

namespace Dynamo {
    InvalidKey::InvalidKey(std::string key, std::string map_name) {
        key_ = key;
        map_name_ = map_name;
        msg_ = key_ + " not found in " + map_name_;
    }

    const char *InvalidKey::what() const throw() {
        return msg_.c_str();
    }   
}
