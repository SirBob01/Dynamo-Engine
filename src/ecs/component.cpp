#include "component.h"

namespace Dynamo {
    unsigned TypeID::increment_id() {
        static unsigned id_counter_;
        return id_counter_++;
    };
}