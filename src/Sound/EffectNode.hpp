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
        std::vector<std::reference_wrapper<EffectNode>> _dependencies;

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
         * @brief Connect a dependency.
         *
         * The resulting graph must not result in a cycle.
         *
         * @param node
         */
        void connect(EffectNode &node);

        /**
         * @brief Disconnect all dependencies.
         *
         */
        void disconnect();

        /**
         * @brief Disconnect a dependency.
         *
         * @param node
         */
        void disconnect(EffectNode &node);

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