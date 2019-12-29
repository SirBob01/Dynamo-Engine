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

    SDL_Rect AABB::convert_to_rect() {
        SDL_Point sdl_dim = dim.convert_to_point();
        SDL_Point sdl_center = (center - dim/2).convert_to_point();
        return {
            sdl_center.x,
            sdl_center.y,
            sdl_dim.x,
            sdl_dim.y
        };
    }
}