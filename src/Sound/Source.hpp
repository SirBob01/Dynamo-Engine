#pragma once

#include <atomic>
#include <functional>
#include <optional>

#include <Math/Vec3.hpp>
#include <Sound/Buffer.hpp>
#include <Sound/Filter.hpp>

namespace Dynamo::Sound {
    /**
     * @brief A playable source with associated processing data.
     *
     */
    class Source {
        BufferRef _buffer;
        std::optional<FilterRef> _filter;

        double _frame;
        double _frame_start;
        double _frame_stop;

        mutable std::atomic_bool _playing;

        std::function<void()> _on_finish;

        friend class Jukebox;

      public:
        /**
         * @brief Position of the source.
         *
         */
        Vec3 position;

        /**
         * @brief Velocity of the source.
         *
         */
        Vec3 velocity;

        /**
         * @brief Construct a new sound source.
         *
         * @param buffer
         * @param filter
         */
        Source(Buffer &buffer, std::optional<FilterRef> filter = {});

        /**
         * @brief Check if the source is playing.
         *
         * @return true
         * @return false
         */
        bool is_playing() const;

        /**
         * @brief Set the seek time relative to the start offset.
         *
         * @param time
         */
        void seek(Seconds time);

        /**
         * @brief Set the start time offset.
         *
         * @param time
         */
        void set_start(Seconds time);

        /**
         * @brief Set the stop time offset.
         *
         * @param time
         */
        void set_stop(Seconds time);

        /**
         * @brief Calculate the stop time offset using a duration from the
         * current start offset.
         *
         * @param time
         */
        void set_duration(Seconds time);

        /**
         * @brief Set the on finish handler.
         *
         * @param handler
         */
        void set_on_finish(std::function<void()> handler);
    };

    using SourceRef = std::reference_wrapper<Source>;
} // namespace Dynamo::Sound