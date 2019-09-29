#include "shape.h"

namespace Dynamo::Physics {
	Shape::Shape(SHAPE_TYPE t) {
		type = t;
		centroid = {0, 0};
		volume = 1;
	}

	SHAPE_TYPE Shape::get_type() {
		return type;
	}

	Vec2D Shape::get_centroid() {
		return centroid;
	}

	float Shape::get_volume() {
		return volume;
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