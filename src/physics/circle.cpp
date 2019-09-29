#include "circle.h"

namespace Dynamo::Physics {
	Circle::Circle(Vec2D center, float radius) : Shape(SHAPE_CIRCLE) {
		centroid_ = center;
		radius_ = radius;
		volume_ = radius * radius * PI;
	}

	float Circle::get_radius() {
		return radius_;
	}

	float Circle::get_inertia(float density) {
		return density * volume_ * radius_ * radius_;
	}

	bool Circle::is_in_bounds(Vec2D local) {
		local -= centroid_;
		return local.length_squared() <= (radius_ * radius_);
	}
}