#include "shape.h"

Physics::Shape::Shape(Physics::SHAPE_TYPE t) {
	type = t;
	centroid = {0, 0};
	volume = 1;
}

Physics::SHAPE_TYPE Physics::Shape::get_type() {
	return type;
}

Physics::Vec2D Physics::Shape::get_centroid() {
	return centroid;
}

float Physics::Shape::get_volume() {
	return volume;
}

float Physics::Shape::get_inertia(float density) {
	// Override;
	// This is more than likely an expensive procedure
	// Only call once
}

bool Physics::Shape::is_in_bounds(Physics::Vec2D local_point) {
	// Override;
}