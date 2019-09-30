#include "edge.h"

namespace Dynamo::Physics {
    Edge::Edge(Vec2D endpoints[2]) : Shape(SHAPE_EDGE) {
        start_ = endpoints[0];
        end_ = endpoints[1];

        Vec2D d = end_ - start_;
        centroid_ = d/2.0;
        
        // Volume is really just one dimensional
        volume_ = d.length();
    }

    float Edge::get_inertia(float density) {
        // Easy one dimensional calculation
        return density * volume_ * volume_ * volume_ / 12.0;
    }
}