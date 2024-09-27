#include <Math/Vectorize.hpp>
#include <Sound/Sound.hpp>

namespace Dynamo::Sound {
    Sound::Sound(unsigned frames, unsigned channels, float sample_rate) :
        ChannelData<WaveSample>(frames, channels), _sample_rate(sample_rate) {}

    Sound::Sound(const std::vector<WaveSample> &samples,
                 unsigned channels,
                 float sample_rate) :
        ChannelData<WaveSample>(samples, channels),
        _sample_rate(sample_rate) {}

    void Sound::vadd_channel(Sound &dst,
                             unsigned src_channel,
                             unsigned dst_channel,
                             unsigned src_offset,
                             unsigned dst_offset,
                             unsigned length) const {
        Vectorize::vadd((*this)[src_channel] + src_offset,
                        dst[dst_channel] + dst_offset,
                        dst[dst_channel] + dst_offset,
                        length);
    }

    void Sound::vsma_channel(Sound &dst,
                             float scalar,
                             unsigned src_channel,
                             unsigned dst_channel,
                             unsigned src_offset,
                             unsigned dst_offset,
                             unsigned length) const {
        Vectorize::vsma((*this)[src_channel] + src_offset,
                        scalar,
                        dst[dst_channel] + dst_offset,
                        length);
    }

    float Sound::sample_rate() const { return _sample_rate; }

    void Sound::remix(Sound &dst,
                      unsigned src_offset,
                      unsigned dst_offset,
                      unsigned length) const {
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
            vadd_channel(dst, 0, 0, src_offset, dst_offset, length);
            vadd_channel(dst, 0, 1, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_1_6:
            // L = 0
            // R = 0
            // C = M
            // LFE = 0
            // SL = 0
            // SR = 0
            vadd_channel(dst, 0, 2, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_2_1:
            // M = 0.5 * (L + R)
            vsma_channel(dst, 0.5, 0, 0, src_offset, dst_offset, length);
            vsma_channel(dst, 0.5, 1, 0, src_offset, dst_offset, length);
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
            vadd_channel(dst, 0, 0, src_offset, dst_offset, length);
            vadd_channel(dst, 1, 1, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_4_1:
            // M = 0.25 * (L + R + SL + SR)
            vsma_channel(dst, 0.25, 0, 0, src_offset, dst_offset, length);
            vsma_channel(dst, 0.25, 1, 0, src_offset, dst_offset, length);
            vsma_channel(dst, 0.25, 2, 0, src_offset, dst_offset, length);
            vsma_channel(dst, 0.25, 3, 0, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_4_2:
            // L = 0.5 * (L + SL)
            // R = 0.5 * (L + SR)
            vsma_channel(dst, 0.5, 0, 0, src_offset, dst_offset, length);
            vsma_channel(dst, 0.5, 2, 0, src_offset, dst_offset, length);

            vsma_channel(dst, 0.5, 1, 1, src_offset, dst_offset, length);
            vsma_channel(dst, 0.5, 3, 1, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_4_6:
            // L = L
            // R = R
            // C = 0
            // LFE = 0
            // SL = SL
            // SR = SR
            vadd_channel(dst, 0, 0, src_offset, dst_offset, length);
            vadd_channel(dst, 1, 1, src_offset, dst_offset, length);

            vadd_channel(dst, 2, 4, src_offset, dst_offset, length);
            vadd_channel(dst, 3, 5, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_6_1:
            // M = 0.7071 * (L + R) + C + 0.5 * (SL + SR)
            vsma_channel(dst, M_SQRT1_2, 0, 0, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 1, 0, src_offset, dst_offset, length);

            vadd_channel(dst, 2, 0, src_offset, dst_offset, length);

            vsma_channel(dst, 0.5, 4, 0, src_offset, dst_offset, length);
            vsma_channel(dst, 0.5, 5, 0, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_6_2:
            // L = L + 0.7071 * (C + input.SL)
            // R = R + 0.7071 * (C + input.SR)
            vadd_channel(dst, 0, 0, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 2, 0, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 4, 0, src_offset, dst_offset, length);

            vadd_channel(dst, 1, 1, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 2, 1, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 5, 1, src_offset, dst_offset, length);
            break;
        case ChannelRemixMode::C_6_4:
            // L = L + 0.7071 * C
            // R = R + 0.7071 * C
            // SL = SL
            // SR = SR
            vadd_channel(dst, 0, 0, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 2, 0, src_offset, dst_offset, length);

            vadd_channel(dst, 1, 1, src_offset, dst_offset, length);
            vsma_channel(dst, M_SQRT1_2, 2, 1, src_offset, dst_offset, length);

            vadd_channel(dst, 4, 2, src_offset, dst_offset, length);
            vadd_channel(dst, 5, 3, src_offset, dst_offset, length);
            break;
        default: {
            // Discrete channel remixing
            unsigned channels = std::min(src_channels, dst_channels);
            for (unsigned c = 0; c < channels; c++) {
                vadd_channel(dst, c, c, src_offset, dst_offset, length);
            }
            break;
        }
        }
    }
} // namespace Dynamo::Sound