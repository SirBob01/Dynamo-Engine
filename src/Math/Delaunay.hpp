#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "../Log/Log.hpp"
#include "./Box2.hpp"
#include "./Circle.hpp"
#include "./Segment2.hpp"
#include "./Triangle2.hpp"
#include "./Vec2.hpp"

namespace Dynamo::Delaunay {
    /**
     * @brief Calculate the delaunay triangulation of a point set
     *
     * Note: This will reorder the points in-place for optimization purposes
     *
     * @param points Input point set
     */
    std::vector<Triangle2> triangulate(std::vector<Vec2> &points);

    /**
     * @brief Calculate the box that contains all points in the set
     *
     * @param points
     * @return Box2
     */
    Box2 calculate_bounding_volume(const std::vector<Vec2> &points);

    /**
     * @brief Calculate the super triangle that contains the box
     *
     * @param box
     * @return Triangle2
     */
    Triangle2 calculate_super_triangle(const Box2 &box);
} // namespace Dynamo::Delaunay