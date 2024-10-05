#include <Sound/Buffer.hpp>
#include <Sound/Source.hpp>
#include <algorithm>

namespace Dynamo::Sound {
    Source::Source(Buffer &buffer, std::optional<FilterRef> filter) :
        _buffer(buffer), _filter(filter), _frame(0), _frame_start(0),
        _frame_stop(_buffer.get().frames()), _playing(false),
        _on_finish([]() {}) {}

    bool Source::is_playing() const { return _playing; }

    void Source::seek(Seconds time) {
        _frame = std::clamp(_frame_start + STANDARD_SAMPLE_RATE * time.count(),
                            0.0,
                            static_cast<double>(_buffer.get().frames()));
    }

    void Source::set_start(Seconds time) {
        _frame_start = std::clamp(STANDARD_SAMPLE_RATE * time.count(),
                                  0.0,
                                  static_cast<double>(_buffer.get().frames()));
    }

    void Source::set_stop(Seconds time) {
        _frame_stop = std::clamp(STANDARD_SAMPLE_RATE * time.count(),
                                 0.0,
                                 static_cast<double>(_buffer.get().frames()));
    }

    void Source::set_duration(Seconds time) {
        float count = STANDARD_SAMPLE_RATE * time.count();
        _frame_stop = std::clamp(_frame_start + count,
                                 0.0,
                                 static_cast<double>(_buffer.get().frames()));
    }

    void Source::set_on_finish(std::function<void()> handler) {
        _on_finish = handler;
    }
} // namespace Dynamo::Sound