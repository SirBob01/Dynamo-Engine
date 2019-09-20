#include "shape.h"

PhysicsShape::PhysicsShape(PHYSICS_SHAPE_TYPE t) {
	type = t;
	centroid = {0, 0};
	volume = 1;
}

PHYSICS_SHAPE_TYPE PhysicsShape::get_type() {
	return type;
}

Vec2D PhysicsShape::get_centroid() {
	return centroid;
}

float PhysicsShape::get_volume() {
	return volume;
}

float PhysicsShape::get_inertia(float density) {
	// Override;
	// This is more than likely an expensive procedure
	// Only call once
}

bool PhysicsShape::is_in_bounds(Vec2D local_point) {
	// Override;
}