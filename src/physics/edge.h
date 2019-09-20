#ifndef GAME_PHYSICS_EDGE
#define GAME_PHYSICS_EDGE

#include "shape.h"
#include "vec2d.h"
#include "common.h"

// TODO: Implement edges once done with polygons and circles
class PhysicsEdge : public PhysicsShape {
	Vec2D start;
	Vec2D end;

public:
	PhysicsEdge(Vec2D endpoints[2]);

	float get_inertia(float density) override;
};

#endif