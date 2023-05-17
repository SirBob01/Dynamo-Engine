#pragma once

#include <functional>
#include <unordered_set>
#include <vector>

#include "../Types.hpp"
#include "./Listener.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Node in a sound effects processing graph.
     *
     */
    class EffectNode {
        std::vector<std::reference_wrapper<EffectNode>> _outgoing;

      private:
        /**
         * @brief Test if the graph has cycles.
         *
         * @return b8
         */
        b8 has_cycles();

      public:
        /**
         * @brief Destroy the EffectNode object.
         *
         */
        virtual ~EffectNode() = default;

        /**
         * @brief Connect an outgoing EffectNode.
         *
         * The resulting graph must not result in a cycle.
         *
         * @param destination
         */
        void connect(EffectNode &destination);

        /**
         * @brief Disconnect all outgoing nodes.
         *
         */
        void disconnect();

        /**
         * @brief Disconnect an outgoing node.
         *
         * @param destination
         */
        void disconnect(EffectNode &destination);

        /**
         * @brief Run the Sound through the graph rooted at this node.
         *
         * @param src
         * @param offset
         * @param length
         * @param listeners
         * @return Sound&
         */
        Sound &run(Sound &src, u32 offset, u32 length, ListenerSet &listeners);

        /**
         * @brief Transform a Sound.
         *
         * @param src
         * @param length
         * @param listeners
         */
        virtual Sound &transform(Sound &src,
                                 u32 offset,
                                 u32 length,
                                 ListenerSet &listeners) = 0;
    };
} // namespace Dynamo::Sound