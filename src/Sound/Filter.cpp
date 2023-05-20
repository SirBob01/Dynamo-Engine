#include "./Filter.hpp"

namespace Dynamo::Sound {
    Filter::Filter(u32 channels) : _channels(channels) {}

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

    Sound &Filter::get_input() { return _input; }

    Sound &Filter::get_output() { return _output; }

    std::vector<std::reference_wrapper<Filter>> topological_sort(Filter &root) {
        std::vector<std::reference_wrapper<Filter>> order;
        std::vector<std::reference_wrapper<Filter>> stack;
        stack.push_back(root);

        // Implement topological sorting algorithm
        while (!stack.empty()) {
            Filter &node = stack.back();
            stack.pop_back();

            for (Filter &child : node.get_dependencies()) {
                stack.push_back(child);
            }
            order.push_back(node);
        }

        return order;
    }

    Sound &run_filter(Filter &filter,
                      Sound &src,
                      u32 offset,
                      u32 length,
                      ListenerSet &listeners) {
        const u32 channels = src.channels();
        auto ordered = topological_sort(filter);

        // Write input buffers
        for (u32 i = 0; i < ordered.size(); i++) {
            Filter &node = ordered[i];
            if (node.get_dependencies().empty()) {
                Sound &input = node.get_input();
                input.resize(length, channels);

                for (u32 c = 0; c < channels; c++) {
                    for (u32 f = 0; f < length; f++) {
                        input[c][f] = src[c][f + offset];
                    }
                }
            }
        }
        for (Filter &node : ordered) {
            // for (Filter &dep : node.get_dependencies()) {
            // }
            // Sound &src = node.get_input();
            // node.transform({listeners});
        }
        return src;
    }
} // namespace Dynamo::Sound