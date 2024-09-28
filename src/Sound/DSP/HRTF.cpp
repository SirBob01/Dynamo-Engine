#include <unordered_map>

#include <Math/Common.hpp>
#include <Math/Delaunay.hpp>
#include <Math/Triangle2.hpp>
#include <Math/Vectorize.hpp>
#include <Sound/DSP/HRTF.hpp>

namespace Dynamo::Sound {
    HRTF::HRTF() {
        // Generate point space
        for (float az : HRTF_AZIMUTHS) {
            _points.emplace_back(az, -90);
            for (unsigned i = 0; i < 50; i++) {
                float el = -45 + 5.625 * i;
                _points.emplace_back(az, el);
            }
            _points.emplace_back(az, 270);
        }
        _coeff_map.resize(_points.size());

        // Map each point to its index
        std::unordered_map<Vec2, unsigned> index_map;
        for (unsigned i = 0; i < _points.size(); i++) {
            index_map[_points[i]] = i;
        }

        // Triangulate points
        // TODO: Register triangles into a quadtree for faster querying
        std::vector<Vec2> tmp_points = _points;
        for (const Triangle2 &triangle : Delaunay::triangulate(tmp_points)) {
            _indices.push_back(index_map[triangle.a]);
            _indices.push_back(index_map[triangle.b]);
            _indices.push_back(index_map[triangle.c]);
        }

        // Read the HRIR samples for each point
        unsigned offset = 0;
        for (unsigned i = 0; i < _points.size(); i++) {
            Sound &coeff = _coeff_map[i];
            coeff.resize(HRIR_LENGTH, 2);
            for (unsigned c = 0; c < 2; c++) {
                const WaveSample *samples = HRIR_COEFFICIENTS.data() + offset;
                std::copy(samples, samples + HRIR_LENGTH, coeff[c]);
                offset += HRIR_LENGTH;
            }
        }
    }

    Vec2 HRTF::compute_point(const Vec3 &listener_position,
                             const Vec3 &source_position) const {
        Vec3 disp = source_position - listener_position;

        float azimuth = std::asin(disp.x / disp.length());
        float elevation = std::atan2(-disp.z, disp.y);
        if (disp.y < 0 && disp.z < 0) {
            elevation += M_2_PI;
        }

        return Vec2(to_degrees(azimuth), to_degrees(elevation));
    }

    void HRTF::calculate_HRIR(const Vec3 &listener_position,
                              const Quaternion &listener_rotation,
                              const Vec3 &source_position,
                              Sound &dst_buffer) const {
        dst_buffer.clear();
        Vec2 point = compute_point(listener_position, source_position);
        for (unsigned t = 0; t < _indices.size(); t += 3) {
            unsigned a = _indices[t];
            unsigned b = _indices[t + 1];
            unsigned c = _indices[t + 2];

            Triangle2 triangle(_points[a], _points[b], _points[c]);
            Vec3 coords = triangle.barycentric(point);

            float eps = 1e-6;
            if (coords.x >= -eps && coords.y >= -eps && coords.z >= -eps) {
                const Sound &ir0 = _coeff_map[a];
                const Sound &ir1 = _coeff_map[b];
                const Sound &ir2 = _coeff_map[c];

                // Use barycentric coordinates to interpolate samples
                for (unsigned c = 0; c < dst_buffer.channels(); c++) {
                    WaveSample *dst = dst_buffer[c];
                    unsigned frames = dst_buffer.frames();
                    Vectorize::vsma(ir0[c], coords.x, dst, frames);
                    Vectorize::vsma(ir1[c], coords.y, dst, frames);
                    Vectorize::vsma(ir2[c], coords.z, dst, frames);
                }
                return;
            }
        }
        Dynamo::Log::error("HRTF could not triagulate ({} {})",
                           point.x,
                           point.y);
    }
} // namespace Dynamo::Sound