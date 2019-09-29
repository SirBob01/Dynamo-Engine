#include "circle.h"

namespace Dynamo::Physics {
	Circle::Circle(Vec2D center, float radius) : Shape(SHAPE_CIRCLE) {
		centroid = center;
		r = radius;
		volume = r * r * PI;
	}

	float Circle::get_radius() {
		return r;
	}

	float Circle::get_inertia(float density) {
		return density * volume * r * r;
	}

	bool Circle::is_in_bounds(Vec2D local) {
		local -= centroid;
		return local.length_squared() <= (r * r);
	}
}