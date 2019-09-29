#ifndef DYNAMO_PHYSICS_CIRCLE_H_
#define DYNAMO_PHYSICS_CIRCLE_H_

#include "shape.h"
#include "vec2d.h"
#include "common.h"

namespace Physics {
	class Circle : public Shape {
		float r;

	public:
		Circle(Vec2D center, float radius);

		float get_radius();

		float get_inertia(float density) override;

		bool is_in_bounds(Vec2D local) override;
	};
}

#endif