#include "HRTF.hpp"

namespace Dynamo::Sound {
    void calculate_HRIR(const Vec3 &listener_position,
                        const Quaternion &listener_rotation,
                        const Vec3 &source_position,
                        Sound &dst_buffer) {
        // Calculate azimuth and elevation angles
        Vec3 disp = source_position - listener_position;
        f32 azimuth = std::asin(disp.x / disp.length());
        f32 elevation = std::atan2(-disp.z, disp.y);
        if (disp.y < 0 && disp.z < 0) {
            elevation += 2 * M_PI;
        }
        Vec2 point(to_degrees(azimuth), to_degrees(elevation));

        // Find the triangle containing the point
        // TODO: Use adjacency walk algorithm
        // * Map each point to the list of its adjacent triangles
        // * Use a quadtree to find closest point
        // * Select triangle by testing for non-negative barycentric coordinates
        Triangle2 triangle({}, {}, {});
        std::vector<Vec2> stack = HRIR_QUADTREE.get_neighbors(point);
        std::unordered_set<Vec2> visited;
        while (!stack.empty()) {
            Vec2 &curr = stack.back();
            stack.pop_back();
            visited.insert(curr);

            for (const Triangle2 &adj : HRIR_TRIANGLE_MAP[curr]) {
                Vec3 coords = adj.barycentric(point);
                if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0) {
                    triangle = adj;
                    break;
                } else {
                    f32 min = std::min(std::min(coords.x, coords.y), coords.z);
                    if (min == coords.x) {
                        if (!visited.count(triangle.b)) {
                            stack.push_back(triangle.b);
                        }
                        if (!visited.count(triangle.c)) {
                            stack.push_back(triangle.c);
                        }
                    }
                    if (min == coords.y) {
                        if (!visited.count(triangle.a)) {
                            stack.push_back(triangle.a);
                        }
                        if (!visited.count(triangle.c)) {
                            stack.push_back(triangle.c);
                        }
                    }
                    if (min == coords.z) {
                        if (!visited.count(triangle.a)) {
                            stack.push_back(triangle.a);
                        }
                        if (!visited.count(triangle.b)) {
                            stack.push_back(triangle.b);
                        }
                    }
                }
            }
        }

        // Interpolate the impulse response
        dst_buffer.clear();
        Vec3 coords = triangle.barycentric(point);
        if (coords.x >= 0 && coords.y >= 0 && coords.z >= 0) {
            Sound &ir0 = HRIR_MAP[triangle.a];
            Sound &ir1 = HRIR_MAP[triangle.b];
            Sound &ir2 = HRIR_MAP[triangle.c];

            // Use barycentric coordinates to interpolate samples
            for (u32 c = 0; c < dst_buffer.channels(); c++) {
                for (u32 f = 0; f < dst_buffer.frames(); f++) {
                    dst_buffer[c][f] = ir0[c][f] * coords.x +
                                       ir1[c][f] * coords.y +
                                       ir2[c][f] * coords.z;
                }
            }
        }
    }
} // namespace Dynamo::Sound