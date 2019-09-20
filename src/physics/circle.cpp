#include "circle.h"

PhysicsCircle::PhysicsCircle(Vec2D center, float radius) : PhysicsShape(PHYSICS_SHAPE_CIRCLE) {
	centroid = center;
	r = radius;
	volume = r * r * PI;
}

float PhysicsCircle::get_radius() {
	return r;
}

float PhysicsCircle::get_inertia(float density) {
	return density * volume * r * r;
}

bool PhysicsCircle::is_in_bounds(Vec2D local) {
	local -= centroid;
	return local.length_squared() <= (r * r);
}