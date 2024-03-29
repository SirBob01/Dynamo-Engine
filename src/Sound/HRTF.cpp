#include "HRTF.hpp"

namespace Dynamo::Sound {
    HRTF::HRTF() {
        // Generate point space
        for (const f32 &a : HRTF_AZIMUTHS) {
            _points.push_back({a, -90});
            for (u32 i = 0; i < 50; i++) {
                f32 e = -45 + 5.625 * i;
                _points.push_back({a, e});
            }
            _points.push_back({a, 270});
        }

        // Map each point to its index
        std::unordered_map<Vec2, u32> index_map;
        for (u32 i = 0; i < _points.size(); i++) {
            index_map[_points[i]] = i;
        }

        // Triangulate points
        std::vector<Vec2> tmp_points = _points;
        for (const Triangle2 &triangle : Delaunay::triangulate(tmp_points)) {
            _indices.push_back(index_map[triangle.a]);
            _indices.push_back(index_map[triangle.b]);
            _indices.push_back(index_map[triangle.c]);
        }

        // Read the HRIR samples for each point
        u32 offset = 0;
        for (u32 i = 0; i < _points.size(); i++) {
            _coeff_map[i].resize(HRIR_LENGTH, 2);
            for (u32 c = 0; c < 2; c++) {
                for (u32 f = 0; f < HRIR_LENGTH; f++) {
                    _coeff_map[i][c][f] = HRIR_COEFFICIENTS[offset++];
                }
            }
        }
    }

    Vec2 HRTF::compute_point(const Vec3 &listener_position,
                             const Vec3 &source_position) {
        Vec3 disp = source_position - listener_position;

        f32 azimuth = std::asin(disp.x / disp.length());
        f32 elevation = std::atan2(-disp.z, disp.y);
        if (disp.y < 0 && disp.z < 0) {
            elevation += 2 * M_PI;
        }

        return Vec2(to_degrees(azimuth), to_degrees(elevation));
    }

    void HRTF::calculate_HRIR(const Vec3 &listener_position,
                              const Quaternion &listener_rotation,
                              const Vec3 &source_position,
                              Sound &dst_buffer) {
        dst_buffer.clear();
        Vec2 point = compute_point(listener_position, source_position);
        for (u32 t = 0; t < _indices.size() / 3; t++) {
            u32 a = _indices[t * 3];
            u32 b = _indices[t * 3 + 1];
            u32 c = _indices[t * 3 + 2];
            Triangle2 triangle(_points[a], _points[b], _points[c]);

            Vec3 coords = triangle.barycentric(point);
            if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0) {
                Sound &ir0 = _coeff_map[a];
                Sound &ir1 = _coeff_map[b];
                Sound &ir2 = _coeff_map[c];

                // Use barycentric coordinates to interpolate samples
                for (u32 c = 0; c < dst_buffer.channels(); c++) {
                    for (u32 f = 0; f < dst_buffer.frames(); f++) {
                        dst_buffer[c][f] = ir0[c][f] * coords.x +
                                           ir1[c][f] * coords.y +
                                           ir2[c][f] * coords.z;
                    }
                }
                break;
            }
        }
    }
} // namespace Dynamo::Sound