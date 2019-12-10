#include "aabb.h"

namespace Dynamo {
    const bool AABB::is_colliding(const AABB &other) const {
        Vec2D min = get_min();
        Vec2D max = get_max();

        Vec2D other_min = other.get_min();
        Vec2D other_max = other.get_max();

        bool hor = other_max.x > min.x && other_min.x < max.x;
        bool ver = other_max.y > min.y && other_min.y < max.y;
        
        return hor && ver;
    }

    const bool AABB::is_in_bounds(const Vec2D &point) const {
        Vec2D min = get_min();
        Vec2D max = get_max();

        bool hor = point.x < max.x && point.x > min.x;
        bool ver = point.y < max.y && point.y > min.y;
        
        return hor && ver;
    }

    const Vec2D AABB::get_min() const {
        return center - (dim/2);
    }

    const Vec2D AABB::get_max() const {
        return center + (dim/2);
    }
}