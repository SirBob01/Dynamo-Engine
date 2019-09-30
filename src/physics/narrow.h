#ifndef DYNAMO_PHYSICS_NARROW_H_
#define DYNAMO_PHYSICS_NARROW_H_

#include "body.h"
#include "polygon.h"
#include "circle.h"
#include "edge.h"
#include "broad.h"
#include "vec2d.h"

namespace Dynamo::Physics {
    struct Manifold {
        // Participating bodies
        Pair *pair;
        
        // Relevant fixtures
        Fixture *a;
        Fixture *b;

        // Collision data
        Vec2D normal; // Normalized direction
        Vec2D contact;
        float penetration;
    };

    // Project shape onto axis for SAT algorithm
    Projection project_polygon(Polygon *poly, Vec2D axis, Body *body);

    Projection project_circle(Circle *circle, Vec2D axis, Body *body);

    Projection project_edge(Edge *edge, Vec2D axis, Body *body);

    // Manifold generation
    bool colliding_polygons(Polygon *a, Polygon *b, 
                            Pair *pair, Manifold *m);

    bool colliding_circles(Circle *a, Circle *b, 
                           Pair *pair, Manifold *m);

    bool colliding_polygon_circle(Polygon *p, Circle *c, 
                                  Pair *pair, Manifold *m);

    // Collision wrapper
    Manifold colliding(Pair *pair, Fixture *a, Fixture *b);

    // Impulse calculation
    void resolve_impulse(Manifold *m);
}

#endif