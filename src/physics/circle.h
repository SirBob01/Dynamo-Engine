#ifndef GAME_PHYSICS_CIRCLE
#define GAME_PHYSICS_CIRCLE

#include "shape.h"
#include "vec2d.h"
#include "common.h"

class PhysicsCircle : public PhysicsShape {
	float r;

public:
	PhysicsCircle(Vec2D center, float radius);

	float get_radius();

	float get_inertia(float density) override;

	bool is_in_bounds(Vec2D local) override;
};

#endif