#include "edge.h"

PhysicsEdge::PhysicsEdge(Vec2D endpoints[2]) : PhysicsShape(PHYSICS_SHAPE_EDGE) {
	start = endpoints[0];
	end = endpoints[1];

	Vec2D d = end - start;
	centroid = d/2.0;
	
	// Volume is really just one dimensional
	volume = d.length();
}

float PhysicsEdge::get_inertia(float density) {
	// Easy one dimensional calculation
	return density * volume * volume * volume / 12.0;
}