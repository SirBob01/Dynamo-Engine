#ifndef DYNAMO_PHYSICS_SHAPE_H_
#define DYNAMO_PHYSICS_SHAPE_H_

#include "vec2d.h"

namespace Dynamo::Physics {
    typedef enum {
        SHAPE_POLYGON,
        SHAPE_CIRCLE,
        SHAPE_EDGE,
        SHAPE_COUNT = 3,
    } SHAPE_TYPE;

    // All points passed here by the Body should be in local coordinates
    class Shape {
    protected:
        SHAPE_TYPE type_;
        Vec2D centroid_;
        float volume_;

    public:
        Shape(SHAPE_TYPE type);

        SHAPE_TYPE get_type();

        // Get the local center of the shape
        Vec2D get_centroid();

        float get_volume();

        virtual float get_inertia(float density);

        // Test if a local point is colliding
        virtual bool is_in_bounds(Vec2D local);
    };
}

#endif