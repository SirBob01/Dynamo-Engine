#include "./EffectNode.hpp"

namespace Dynamo::Sound {
    b8 EffectNode::has_cycles() {
        std::unordered_set<EffectNode *> visited;
        std::vector<std::reference_wrapper<EffectNode>> stack;

        stack.push_back(*this);
        while (!stack.empty()) {
            EffectNode &node = stack.back();
            stack.pop_back();
            if (visited.count(&node) > 0) {
                return true;
            }
            for (EffectNode &child : node._dependencies) {
                stack.push_back(child);
            }
            visited.insert(&node);
        }
        return false;
    }

    void EffectNode::connect(EffectNode &destination) {
        _dependencies.push_back(destination);
        if (has_cycles()) {
            Log::error("Jukebox EffectNode graph disallows cycles.");
        }
    }

    void EffectNode::disconnect() { _dependencies.clear(); }

    void EffectNode::disconnect(EffectNode &node) {
        auto it = _dependencies.begin();
        while (it != _dependencies.end()) {
            EffectNode &curr = *it;
            if (&curr == &node) {
                it = _dependencies.erase(it);
                return;
            } else {
                it++;
            }
        }
        Log::error("Jukebox EffectNode graph attempted to disconnect a "
                   "non-existent node.");
    }

    Sound &EffectNode::run(Sound &src,
                           u32 offset,
                           u32 length,
                           ListenerSet &listeners) {
        // TODO: Implement
        return transform(src, offset, length, listeners);
    }
} // namespace Dynamo::Sound