#pragma once

#include <functional>
#include <limits>
#include <unordered_set>
#include <vector>

#include "../Types.hpp"
#include "./Listener.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Filter context for performing transformations.
     *
     */
    struct FilterContext {
        /**
         * @brief Input buffer.
         *
         */
        std::reference_wrapper<Sound> input;

        /**
         * @brief Output buffer.
         *
         */
        std::reference_wrapper<Sound> output;

        /**
         * @brief Set of all listeners.
         *
         */
        std::reference_wrapper<ListenerSet> listeners;
    };

    /**
     * @brief Filter node in a signal processing graph.
     *
     */
    class Filter {
        Sound _input;
        Sound _output;
        u32 _channels;

        std::vector<std::reference_wrapper<Filter>> _dependencies;

      private:
        /**
         * @brief Test if the graph has cycles.
         *
         * @return b8
         */
        b8 has_cycles();

      public:
        /**
         * @brief Construct a new Filter object.
         *
         * @param channels Number of channels to process.
         */
        Filter(u32 channels = 0);

        /**
         * @brief Destroy the Filter object.
         *
         */
        virtual ~Filter() = default;

        /**
         * @brief Connect a dependency.
         *
         * The resulting graph must not result in a cycle.
         *
         * @param node
         */
        void connect(Filter &node);

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
        void disconnect(Filter &node);

        /**
         * @brief Get the immutable list of dependencies.
         *
         * @return const std::vector<std::reference_wrapper<Filter>>&
         */
        const std::vector<std::reference_wrapper<Filter>> &
        get_dependencies() const;

        /**
         * @brief Get the input buffer.
         *
         * @return Sound&
         */
        Sound &get_input();

        /**
         * @brief Get the output buffer.
         *
         * @return Sound&
         */
        Sound &get_output();

        /**
         * @brief Get the number of channels that can be processed.
         *
         * Input buffers will be upmixed or downmixed to match this value.
         *
         * @return u32
         */
        u32 get_channels();

        /**
         * @brief Transform a Sound.
         *
         * @param context Filter context data.
         */
        virtual void transform(FilterContext context) = 0;
    };

    /**
     * @brief Topologically sort the filter graph.
     *
     * @param root
     * @return std::vector<std::reference_wrapper<Filter>>
     */
    std::vector<std::reference_wrapper<Filter>> topological_sort(Filter &root);

    /**
     * @brief Traverse the filter graph and process the sound.
     *
     * @param filter
     * @param src
     * @param offset
     * @param length
     * @param listeners
     * @return Sound&
     */
    Sound &run_filter(Filter &root,
                      Sound &src,
                      u32 offset,
                      u32 length,
                      ListenerSet &listeners);
} // namespace Dynamo::Sound