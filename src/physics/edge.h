#ifndef DYNAMO_PHYSICS_EDGE_H_
#define DYNAMO_PHYSICS_EDGE_H_

#include "shape.h"
#include "vec2d.h"
#include "common.h"

namespace Physics {
	// TODO: Implement edges once done with polygons and circles
	class Edge : public Shape {
		Vec2D start;
		Vec2D end;

	public:
		Edge(Vec2D endpoints[2]);

		float get_inertia(float density) override;
	};
}

#endif