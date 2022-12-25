#include "HRTF.hpp"

namespace Dynamo {
    HRTF::HRTF(std::string filename) {
        // Open the file
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<float> buffer(size / sizeof(float));
        if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
            Log::error("HRIR dataset file not found {}.", filename);
        }

        // Generate the point space
        constexpr std::array<float, 27> azimuths = {
            -90, -80, -65, -55, -45, -40, -35, -30, -25, -20, -15, -10, -5, 0,
            5,   10,  15,  20,  25,  30,  35,  40,  45,  55,  65,  80,  90,
        };
        std::vector<Vec2> points;
        for (const float &a : azimuths) {
            points.push_back({a, -90});
            for (unsigned i = 0; i < 50; i++) {
                float e = -45 + 5.625 * i;
                points.push_back({a, e});
            }
            points.push_back({a, 270});
        }

        // Read the HRIR coefficients for each point
        _filter_length = 200;
        unsigned offset = 0;
        for (Vec2 &point : points) {
            _coeffs[point].resize(_filter_length, 2);
            for (unsigned c = 0; c < 2; c++) {
                for (unsigned f = 0; f < _filter_length; f++) {
                    _coeffs[point][c][f] = buffer[offset++];
                }
            }
        }

        // Triangulate the points
        _triangles = Delaunay::triangulate(points);
    }

    unsigned HRTF::get_filter_length() { return _filter_length; }

    void HRTF::get_coefficients(const Vec3 &listener_position,
                                const Quaternion &listener_rotation,
                                const Vec3 &source_position,
                                ChannelData<Complex> &dst_buffer) {
        // Calculate azimuth and elevation angles
        Vec3 disp = source_position - listener_position;
        float azimuth = std::asin(disp.x / disp.length());
        float elevation = std::atan2(-disp.z, disp.y);
        if (disp.y < 0 && disp.z < 0) {
            elevation += 2 * M_PI;
        }
        Vec2 point(to_degrees(azimuth), to_degrees(elevation));

        // Find the triangle containing the point
        // TODO: Use adjacency walk algorithm
        // * Map each point to the list of its adjacent triangles
        // * Use a quadtree to find closest point
        // * Select triangle by testing for non-negative barycentric coordinates
        dst_buffer.clear();
        for (const Triangle2 &triangle : _triangles) {
            Vec3 coords = triangle.barycentric(point);
            if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0) {
                ChannelData<float> &coeffs0 = _coeffs[triangle.a];
                ChannelData<float> &coeffs1 = _coeffs[triangle.b];
                ChannelData<float> &coeffs2 = _coeffs[triangle.c];

                // Use barycentric coordinates to interpolate coefficients
                for (unsigned c = 0; c < dst_buffer.channels(); c++) {
                    for (unsigned f = 0; f < dst_buffer.frames(); f++) {
                        dst_buffer[c][f].re = coeffs0[c][f] * coords.x +
                                              coeffs1[c][f] * coords.y +
                                              coeffs2[c][f] * coords.z;
                    }
                }
                return;
            }
        }
    }
} // namespace Dynamo