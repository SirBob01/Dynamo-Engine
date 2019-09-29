#include "edge.h"

Physics::Edge::Edge(Physics::Vec2D endpoints[2]) : Physics::Shape(Physics::SHAPE_EDGE) {
	start = endpoints[0];
	end = endpoints[1];

	Vec2D d = end - start;
	centroid = d/2.0;
	
	// Volume is really just one dimensional
	volume = d.length();
}

float Physics::Edge::get_inertia(float density) {
	// Easy one dimensional calculation
	return density * volume * volume * volume / 12.0;
}