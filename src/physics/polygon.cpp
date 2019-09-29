#include "polygon.h"

Physics::Polygon::Polygon(Physics::Vec2D *verts, int n) : Physics::Shape(Physics::SHAPE_POLYGON) {
	// Set vertexes
	vertexes = new Vec2D[n];
	for(int i = 0; i < n; i++) {
		vertexes[i] = verts[i];
	}

	// Calculate normals
	normals = new Vec2D[n];
	for(int i = 0; i < n; i++) {
		Vec2D edge = vertexes[(i+1)%n] - vertexes[i];
		normals[i] = edge.left_normal();
	}

	// Calculate volume
	volume = 0;
	for(int i = 0; i < n; i++) {
		Vec2D a = vertexes[i];
		Vec2D b = vertexes[(i+1)%n];
		volume += (a.x + b.x) * (a.y - b.y);
	}
	volume = abs(volume);
	volume /= 2;

	// Calculate centroid
	centroid = {0, 0};
	for(int i = 0; i < n; i++) {
		Vec2D a = vertexes[(i+1)%n];
		Vec2D b = vertexes[i];

		centroid.x += (a.x + b.x)*((a.x * b.y) - (a.y * b.x));
		centroid.y += (a.y + b.y)*((a.x * b.y) - (a.y * b.x));
	}
	centroid /= (6 * volume);
	
	sides = n;
}

Physics::Polygon::~Polygon() {
	delete vertexes;
	delete normals;
}

int Physics::Polygon::get_sides() {
	return sides;
}

Physics::Vec2D *Physics::Polygon::get_vertexes() {
	return vertexes;
}

Physics::Vec2D *Physics::Polygon::get_normals() {
	return normals;
}

float Physics::Polygon::get_inertia(float density) {
	float I = 0.0;

	for(int i = 0; i < sides; i++) {
		Vec2D a = vertexes[i] - centroid;
		Vec2D b = vertexes[(i+1)%sides] - centroid;
		
		float D = a.cross(b);
		float x2 = a.x*a.x + b.x*a.x + b.x*b.x;
		float y2 = a.y*a.y + b.y*a.y + b.y*b.y;

		I += D * (x2 + y2);
	}

	return I * density / 12.0;
}

bool Physics::Polygon::is_in_bounds(Physics::Vec2D local) {
	// Winding number algorithm
	int wn = 0;
	for(int i = 0; i < sides; i++) {
		Vec2D a = vertexes[i];
		Vec2D b = vertexes[(i+1)%sides];

		float det = (b.x - a.x) * (local.y - a.y) - (b.y - a.y) * (local.x - a.x);
		if(a.y <= local.y) {
			if(b.y > local.y and det > 0) {
				++wn;
			}
		}
		else if(b.y <= local.y) {
			if(det < 0) {
				--wn;
			}
		}
	}
	return (wn != 0);
}