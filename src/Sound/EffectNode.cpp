#include "./EffectNode.hpp"

namespace Dynamo::Sound {
    b8 EffectNode::has_cycles() {
        std::unordered_set<EffectNode *> visited;
        std::vector<std::reference_wrapper<EffectNode>> stack;
        while (!stack.empty()) {
            EffectNode &node = stack.back();
            stack.pop_back();
            if (visited.count(&node) > 0) {
                return true;
            }
            for (EffectNode &child : node._outgoing) {
                stack.push_back(child);
            }
        }
        return false;
    }

    void EffectNode::connect(EffectNode &destination) {
        _outgoing.push_back(destination);
        if (has_cycles()) {
            Log::error("Jukebox EffectNode graph disallows cycles.");
        }
    }

    void EffectNode::disconnect() { _outgoing.clear(); }

    void EffectNode::disconnect(EffectNode &destination) {
        auto it = _outgoing.begin();
        while (it != _outgoing.end()) {
            EffectNode &node = *it;
            if (&node == &destination) {
                it = _outgoing.erase(it);
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
        return process(src, offset, length, listeners);
    }
} // namespace Dynamo::Sound