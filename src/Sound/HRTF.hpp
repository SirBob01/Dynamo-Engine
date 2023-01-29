#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Types.hpp"
#include "../Math/Common.hpp"
#include "../Math/Complex.hpp"
#include "../Math/Delaunay.hpp"
#include "../Math/Quaternion.hpp"
#include "../Math/Triangle2.hpp"
#include "../Math/Vec2.hpp"
#include "../Math/Vec3.hpp"
#include "../Utils/ChannelData.hpp"

#include "./Convolver.hpp"
#include "./Data/HRIR.hpp"
#include "./Sound.hpp"

namespace Dynamo::Sound {
    /**
     * @brief Read the raw HRIR binary data
     *
     * @return WaveForm
     */
    static WaveForm read_HRIR_coefficients() {
        WaveForm buffer(HRIR_bin_len / sizeof(f32));
        std::memcpy(buffer.data(), HRIR_bin, HRIR_bin_len);
        return buffer;
    }

    /**
     * @brief HRIR coefficients array
     *
     */
    static const WaveForm HRIR_COEFFICIENTS = read_HRIR_coefficients();

    /**
     * @brief Length of an HRIR
     *
     */
    static constexpr u32 HRIR_LENGTH = 200;

    /**
     * @brief Generate the HRIR mapping
     *
     * @return std::unordered_map<Vec2, Sound>
     */
    static std::unordered_map<Vec2, Sound> generate_HRIR_map() {
        // Generate the point space
        std::array<f32, 27> azimuths = {
            -90, -80, -65, -55, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0,
            5,   10,  15,  20,  25,  30,  35,  40,  45,  55,  65,  80,  90,
        };
        std::vector<Vec2> points;
        for (const f32 &a : azimuths) {
            points.push_back({a, -90});
            for (u32 i = 0; i < 50; i++) {
                f32 e = -45 + 5.625 * i;
                points.push_back({a, e});
            }
            points.push_back({a, 270});
        }

        // Read the HRIR samples for each point
        std::unordered_map<Vec2, Sound> map;
        u32 offset = 0;
        for (Vec2 &point : points) {
            map[point].resize(HRIR_LENGTH, 2);
            for (u32 c = 0; c < 2; c++) {
                for (u32 f = 0; f < HRIR_LENGTH; f++) {
                    map[point][c][f] = HRIR_COEFFICIENTS[offset++];
                }
            }
        }
        return map;
    }

    /**
     * @brief Map of azimuth-elevation points to HRIR
     *
     */
    static std::unordered_map<Vec2, Sound> HRIR_MAP = generate_HRIR_map();

    /**
     * @brief Generate the azimuth-elevation point triangles
     *
     * @return std::vector<Triangle2>
     */
    static std::vector<Triangle2> generate_HRIR_triangles() {
        std::vector<Vec2> points;
        for (const auto &pair : HRIR_MAP) {
            points.push_back(pair.first);
        }
        return Delaunay::triangulate(points);
    }

    /**
     * @brief Aziumth-elevation point triangles for HRIR interpolation
     *
     */
    static std::vector<Triangle2> HRIR_TRIANGLES = generate_HRIR_triangles();

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
                        Sound &dst_buffer);
} // namespace Dynamo::Sound