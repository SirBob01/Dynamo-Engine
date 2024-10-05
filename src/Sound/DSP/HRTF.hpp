#pragma once

#include <vector>

#include <Math/Quaternion.hpp>
#include <Math/Vec2.hpp>
#include <Math/Vec3.hpp>

#include <Sound/Buffer.hpp>
#include <Sound/Data/HRIR.hpp>

namespace Dynamo::Sound {
    /**
     * @brief Read the raw HRIR binary data
     *
     * @return std::vector<WaveSample>
     */
    static std::vector<WaveSample> read_HRIR_coefficients() {
        std::vector<WaveSample> buffer(HRIR_bin_len / sizeof(float));
        std::memcpy(buffer.data(), HRIR_bin, HRIR_bin_len);
        return buffer;
    }

    /**
     * @brief HRIR coefficients array
     *
     */
    static const std::vector<WaveSample> HRIR_COEFFICIENTS =
        read_HRIR_coefficients();

    /**
     * @brief Length of an HRIR
     *
     */
    static constexpr unsigned HRIR_LENGTH = 200;

    /**
     * @brief HRTF point space x-coordinates
     *
     */
    static constexpr std::array<float, 27> HRTF_AZIMUTHS = {
        -90, -80, -65, -55, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0,
        5,   10,  15,  20,  25,  30,  35,  40,  45,  55,  65,  80,  90,
    };

    /**
     * @brief Head-related transfer function computes impulse response
     * coefficients at a point
     *
     */
    class HRTF {
        std::vector<Vec2> _points;
        std::vector<unsigned> _indices;

        std::vector<Buffer> _coeff_map;

        /**
         * @brief Compute the azimuth and elevation angles
         *
         * @param listener_position Position of the listener
         * @param source_position   Position of the sound source
         * @return Vec2
         */
        Vec2 compute_point(const Vec3 &listener_position,
                           const Vec3 &source_position) const;

      public:
        /**
         * @brief Construct a new HRTF object
         *
         */
        HRTF();

        /**
         * @brief Calculate the head-related impulse response for a sound source
         * relative to the listener, applying interpolation as needed
         *
         * @param listener_position Position of the listener
         * @param listener_rotation Rotation of the listener
         * @param source_position   Position of the sound source
         * @param dst_buffer        Destination buffer
         */
        void calculate_HRIR(const Vec3 &listener_position,
                            const Quaternion &listener_rotation,
                            const Vec3 &source_position,
                            Buffer &dst) const;
    };
} // namespace Dynamo::Sound