#include "Delaunay.hpp"

namespace Dynamo::Delaunay {
    std::vector<Triangle2> triangulate(std::vector<Vec2> &points) {
        DYN_ASSERT(points.size() >= 3);
        std::vector<Triangle2> triangles;

        // Presorting will improve early cutoff rate
        Box2 bounds = calculate_bounding_volume(points);
        if (bounds.width() > bounds.height()) {
            auto comp = [](const Vec2 &a, const Vec2 &b) { return a.x < b.x; };
            std::sort(points.begin(), points.end(), comp);
        } else {
            auto comp = [](const Vec2 &a, const Vec2 &b) { return a.y < b.y; };
            std::sort(points.begin(), points.end(), comp);
        }

        // Calculate the initial super triangle
        Triangle2 super_triangle = calculate_super_triangle(bounds);

        points.push_back(super_triangle.a);
        points.push_back(super_triangle.b);
        points.push_back(super_triangle.c);

        triangles.push_back(super_triangle);

        // Edge buffer
        std::unordered_map<Segment2, int> edges;

        for (const Vec2 &point : points) {
            // Find triangles whose circumcircle contains the query point
            std::vector<Triangle2>::iterator it = triangles.begin();
            while (it != triangles.end()) {
                Triangle2 &triangle = *it;
                Circle circumcircle = triangle.circumcircle();
                if (circumcircle.contains(point)) {
                    Segment2 a(triangle.a, triangle.b);
                    Segment2 b(triangle.b, triangle.c);
                    Segment2 c(triangle.c, triangle.a);
                    if (!edges.count(a)) {
                        edges[a] = 1;
                    } else {
                        edges[a]++;
                    }
                    if (!edges.count(b)) {
                        edges[b] = 1;
                    } else {
                        edges[b]++;
                    }
                    if (!edges.count(c)) {
                        edges[c] = 1;
                    } else {
                        edges[c]++;
                    }
                    it = triangles.erase(it);
                } else {
                    it++;
                }
            }

            // Add triangles formed between the point and the outer edges
            for (const auto &pair : edges) {
                if (pair.second > 1) {
                    continue;
                }
                const Segment2 &edge = pair.first;
                triangles.push_back(Triangle2(edge.a, edge.b, point));
            }

            // Reset the edge buffer
            edges.clear();
        }

        // Remove any triangles which use vertices from super triangle
        std::vector<Triangle2>::iterator it = triangles.begin();
        while (it != triangles.end()) {
            Triangle2 &triangle = *it;
            if (triangle.shared_vertices(super_triangle)) {
                it = triangles.erase(it);
            } else {
                it++;
            }
        }

        // Remove super triangle vertices from the point list
        points.resize(points.size() - 3);

        return triangles;
    }

    Box2 calculate_bounding_volume(const std::vector<Vec2> &points) {
        // Calculate min and max bounds
        Vec2 min = points[0];
        Vec2 max = points[0];
        for (const Vec2 &point : points) {
            if (point.x < min.x) min.x = point.x;
            if (point.x > max.x) max.x = point.x;

            if (point.y < min.y) min.y = point.y;
            if (point.y > max.y) max.y = point.y;
        }

        return Box2(min, max);
    }

    Triangle2 calculate_super_triangle(const Box2 &box) {
        Vec2 center = box.center();
        Triangle2 triangle(center, center, center);

        Vec2 range_2 = (box.max - box.min) * 2;
        triangle.a.x -= range_2.x;
        triangle.a.y -= range_2.y;

        triangle.b.x += range_2.x;
        triangle.b.y -= range_2.y;

        triangle.c.y += range_2.y;

        return triangle;
    }
} // namespace Dynamo::Delaunay