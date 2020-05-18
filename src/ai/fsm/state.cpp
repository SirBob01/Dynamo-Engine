#include "state.h"

namespace Dynamo::AI {
    FiniteState::FiniteState() {
        next_ = nullptr;
    }

    void FiniteState::set_next(FiniteState *next) {
        next_ = next;
    }

    void FiniteState::on_entry() {
        return;
    }

    void FiniteState::on_exit() {
        return;
    }

    void FiniteState::on_run(unsigned dt) {
        return;
    }
}