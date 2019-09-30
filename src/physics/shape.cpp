#include "shape.h"

namespace Dynamo::Physics {
    Shape::Shape(SHAPE_TYPE type) {
        type_ = type;
        centroid_ = {0, 0};
        volume_ = 1;
    }

    SHAPE_TYPE Shape::get_type() {
        return type_;
    }

    Vec2D Shape::get_centroid() {
        return centroid_;
    }

    float Shape::get_volume() {
        return volume_;
    }

    float Shape::get_inertia(float density) {
        // Override;
        // This is more than likely an expensive procedure
        // Only call once
    }

    bool Shape::is_in_bounds(Vec2D local_point) {
        // Override;
    }
}