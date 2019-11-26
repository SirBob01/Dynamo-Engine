#ifndef DYNAMO_AABB_H_
#define DYNAMO_AABB_H_

#include "vec2d.h"

namespace Dynamo {
    struct AABB {
        Vec2D center;
        Vec2D dim;

        const bool is_colliding(const AABB &other) const;
        const bool is_in_bounds(const Vec2D &point) const;
        
        const Vec2D get_min() const;
        const Vec2D get_max() const;
    };
}

#endif