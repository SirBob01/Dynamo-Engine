#include "HRTF.hpp"

namespace Dynamo::Sound {
    void calculate_HRIR(const Vec3 &listener_position,
                        const Quaternion &listener_rotation,
                        const Vec3 &source_position,
                        Sound &dst_buffer) {
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
        for (const Triangle2 &triangle : HRIR_TRIANGLES) {
            Vec3 coords = triangle.barycentric(point);
            if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0) {
                Sound &ir0 = HRIR_MAP[triangle.a];
                Sound &ir1 = HRIR_MAP[triangle.b];
                Sound &ir2 = HRIR_MAP[triangle.c];

                // Use barycentric coordinates to interpolate samples
                for (unsigned c = 0; c < dst_buffer.channels(); c++) {
                    for (unsigned f = 0; f < dst_buffer.frames(); f++) {
                        dst_buffer[c][f] = ir0[c][f] * coords.x +
                                           ir1[c][f] * coords.y +
                                           ir2[c][f] * coords.z;
                    }
                }
                return;
            }
        }
    }
} // namespace Dynamo::Sound