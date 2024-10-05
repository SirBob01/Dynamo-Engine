#include <Math/Vectorize.hpp>
#include <Sound/Buffer.hpp>
#include <algorithm>
#include <new>

namespace Dynamo::Sound {
    void vadd_channel(const Buffer &src,
                      Buffer &dst,
                      unsigned src_channel,
                      unsigned dst_channel) {
        Vectorize::vadd(src[src_channel],
                        dst[dst_channel],
                        dst[dst_channel],
                        src.frames());
    }

    void vsma_channel(const Buffer &src,
                      Buffer &dst,
                      float scalar,
                      unsigned src_channel,
                      unsigned dst_channel) {
        Vectorize::vsma(src[src_channel],
                        scalar,
                        dst[dst_channel],
                        src.frames());
    }

    Buffer::Buffer(unsigned frames, unsigned channels) :
        _frames(frames), _channels(channels) {
        unsigned length = std::max(frames * channels, 1U);
        _samples = new (std::align_val_t(64)) WaveSample[length];
    }

    Buffer::Buffer(WaveSample *samples, unsigned frames, unsigned channels) :
        Buffer(frames, channels) {
        std::copy(samples, samples + (_frames * _channels), _samples);
    }

    Buffer::Buffer(const Buffer &rhs) : Buffer(rhs._frames, rhs._channels) {
        std::copy(rhs._samples, rhs._samples + (_frames * _channels), _samples);
    }

    Buffer::~Buffer() { delete[] _samples; }

    Buffer &Buffer::operator=(const Buffer &rhs) {
        unsigned prev_size = _frames * _channels;
        unsigned next_size = rhs._frames * rhs._channels;

        // Reallocate the sample container if necessary
        if (next_size > prev_size) {
            delete[] _samples;
            _samples = new (std::align_val_t(64)) WaveSample[next_size];
        }

        _frames = rhs._frames;
        _channels = rhs._channels;
        std::copy(rhs._samples, rhs._samples + next_size, _samples);

        return *this;
    }

    WaveSample *Buffer::operator[](const unsigned channel) {
        DYN_ASSERT(channel < _channels);
        return _samples + (_frames * channel);
    }

    const WaveSample *Buffer::operator[](const unsigned channel) const {
        DYN_ASSERT(channel < _channels);
        return _samples + (_frames * channel);
    }

    WaveSample *Buffer::data() { return _samples; }

    const WaveSample *Buffer::data() const { return _samples; }

    unsigned Buffer::frames() const { return _frames; }

    unsigned Buffer::channels() const { return _channels; }

    void Buffer::silence() {
        std::fill(_samples, _samples + (_frames * _channels), 0);
    }

    void Buffer::resize(const unsigned frames, const unsigned channels) {
        unsigned prev_size = _frames * _channels;
        unsigned next_size = frames * channels;

        // Reallocate the sample container if necessary
        if (next_size > prev_size) {
            delete[] _samples;
            _samples = new (std::align_val_t(64)) WaveSample[next_size];
        }

        _frames = frames;
        _channels = channels;
    }

    void Buffer::remix(Buffer &dst) const {
        unsigned src_channels = channels();
        unsigned dst_channels = dst.channels();

        // Handle different channel combinations
        unsigned u = (1 << (src_channels + 7)) | (1 << (dst_channels - 1));
        switch (static_cast<ChannelRemixMode>(u)) {
        case ChannelRemixMode::C_1_2:
            // L = M
            // R = M
            // SL = 0
            // SR = 0
        case ChannelRemixMode::C_1_4:
            // L = M
            // R = M
            // SL = 0
            // SR = 0
            vadd_channel((*this), dst, 0, 0);
            vadd_channel((*this), dst, 0, 1);
            break;
        case ChannelRemixMode::C_1_6:
            // L = 0
            // R = 0
            // C = M
            // LFE = 0
            // SL = 0
            // SR = 0
            vadd_channel((*this), dst, 0, 2);
            break;
        case ChannelRemixMode::C_2_1:
            // M = 0.5 * (L + R)
            vsma_channel((*this), dst, 0.5, 0, 0);
            vsma_channel((*this), dst, 0.5, 1, 0);
            break;
        case ChannelRemixMode::C_2_4:
            // L = L
            // R = R
            // SL = 0
            // SR = 0
        case ChannelRemixMode::C_2_6:
            // L = L
            // R = R
            // C = 0
            // LFE = 0
            // SL = 0
            // SR = 0
            vadd_channel((*this), dst, 0, 0);
            vadd_channel((*this), dst, 1, 1);
            break;
        case ChannelRemixMode::C_4_1:
            // M = 0.25 * (L + R + SL + SR)
            vsma_channel((*this), dst, 0.25, 0, 0);
            vsma_channel((*this), dst, 0.25, 1, 0);
            vsma_channel((*this), dst, 0.25, 2, 0);
            vsma_channel((*this), dst, 0.25, 3, 0);
            break;
        case ChannelRemixMode::C_4_2:
            // L = 0.5 * (L + SL)
            // R = 0.5 * (L + SR)
            vsma_channel((*this), dst, 0.5, 0, 0);
            vsma_channel((*this), dst, 0.5, 2, 0);

            vsma_channel((*this), dst, 0.5, 1, 1);
            vsma_channel((*this), dst, 0.5, 3, 1);
            break;
        case ChannelRemixMode::C_4_6:
            // L = L
            // R = R
            // C = 0
            // LFE = 0
            // SL = SL
            // SR = SR
            vadd_channel((*this), dst, 0, 0);
            vadd_channel((*this), dst, 1, 1);

            vadd_channel((*this), dst, 2, 4);
            vadd_channel((*this), dst, 3, 5);
            break;
        case ChannelRemixMode::C_6_1:
            // M = 0.7071 * (L + R) + C + 0.5 * (SL + SR)
            vsma_channel((*this), dst, M_SQRT1_2, 0, 0);
            vsma_channel((*this), dst, M_SQRT1_2, 1, 0);

            vadd_channel((*this), dst, 2, 0);

            vsma_channel((*this), dst, 0.5, 4, 0);
            vsma_channel((*this), dst, 0.5, 5, 0);
            break;
        case ChannelRemixMode::C_6_2:
            // L = L + 0.7071 * (C + input.SL)
            // R = R + 0.7071 * (C + input.SR)
            vadd_channel((*this), dst, 0, 0);
            vsma_channel((*this), dst, M_SQRT1_2, 2, 0);
            vsma_channel((*this), dst, M_SQRT1_2, 4, 0);

            vadd_channel((*this), dst, 1, 1);
            vsma_channel((*this), dst, M_SQRT1_2, 2, 1);
            vsma_channel((*this), dst, M_SQRT1_2, 5, 1);
            break;
        case ChannelRemixMode::C_6_4:
            // L = L + 0.7071 * C
            // R = R + 0.7071 * C
            // SL = SL
            // SR = SR
            vadd_channel((*this), dst, 0, 0);
            vsma_channel((*this), dst, M_SQRT1_2, 2, 0);

            vadd_channel((*this), dst, 1, 1);
            vsma_channel((*this), dst, M_SQRT1_2, 2, 1);

            vadd_channel((*this), dst, 4, 2);
            vadd_channel((*this), dst, 5, 3);
            break;
        default: {
            // Discrete channel remixing
            unsigned channels = std::min(src_channels, dst_channels);
            for (unsigned c = 0; c < channels; c++) {
                vadd_channel((*this), dst, c, c);
            }
            break;
        }
        }
    }
} // namespace Dynamo::Sound