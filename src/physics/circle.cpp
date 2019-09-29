#include "circle.h"

Physics::Circle::Circle(Physics::Vec2D center, float radius) : Physics::Shape(Physics::SHAPE_CIRCLE) {
	centroid = center;
	r = radius;
	volume = r * r * PI;
}

float Physics::Circle::get_radius() {
	return r;
}

float Physics::Circle::get_inertia(float density) {
	return density * volume * r * r;
}

bool Physics::Circle::is_in_bounds(Physics::Vec2D local) {
	local -= centroid;
	return local.length_squared() <= (r * r);
}