#include "./Filter.hpp"

namespace Dynamo::Sound {
    b8 Filter::has_cycles() {
        std::unordered_set<Filter *> visited;
        std::vector<std::reference_wrapper<Filter>> stack;

        stack.push_back(*this);
        while (!stack.empty()) {
            Filter &node = stack.back();
            stack.pop_back();
            if (visited.count(&node) > 0) {
                return true;
            }
            for (Filter &child : node._dependencies) {
                stack.push_back(child);
            }
            visited.insert(&node);
        }
        return false;
    }

    void Filter::connect(Filter &destination) {
        _dependencies.push_back(destination);
        if (has_cycles()) {
            Log::error("Jukebox Filter graph disallows cycles.");
        }
    }

    void Filter::disconnect(Filter &node) {
        auto it = _dependencies.begin();
        while (it != _dependencies.end()) {
            Filter &curr = *it;
            if (&curr == &node) {
                it = _dependencies.erase(it);
                return;
            } else {
                it++;
            }
        }
        Log::error("Jukebox Filter graph attempted to disconnect a "
                   "non-existent node.");
    }

    void Filter::disconnect() { _dependencies.clear(); }

    const std::vector<std::reference_wrapper<Filter>> &
    Filter::get_dependencies() const {
        return _dependencies;
    }

    Sound &Filter::get_output() { return _output; }

    Sound &run_filter(Filter &filter,
                      Sound &src,
                      u32 offset,
                      u32 length,
                      ListenerSet &listeners) {
        return src;
    }
} // namespace Dynamo::Sound