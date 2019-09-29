#include "polygon.h"

namespace Dynamo::Physics {
	Polygon::Polygon(Vec2D *vertexes, int n) : Shape(SHAPE_POLYGON) {
		// Set vertexes
		vertexes_ = new Vec2D[n];
		for(int i = 0; i < n; i++) {
			vertexes_[i] = vertexes[i];
		}

		// Calculate normals
		normals_ = new Vec2D[n];
		for(int i = 0; i < n; i++) {
			Vec2D edge = vertexes_[(i+1)%n] - vertexes_[i];
			normals_[i] = edge.left_normal();
		}

		// Calculate volume
		volume_ = 0;
		for(int i = 0; i < n; i++) {
			Vec2D a = vertexes_[i];
			Vec2D b = vertexes_[(i+1)%n];
			volume_ += (a.x + b.x) * (a.y - b.y);
		}
		volume_ = abs(volume_);
		volume_ /= 2;

		// Calculate centroid
		centroid_ = {0, 0};
		for(int i = 0; i < n; i++) {
			Vec2D a = vertexes_[(i+1)%n];
			Vec2D b = vertexes_[i];

			centroid_.x += (a.x + b.x)*((a.x * b.y) - (a.y * b.x));
			centroid_.y += (a.y + b.y)*((a.x * b.y) - (a.y * b.x));
		}
		centroid_ /= (6 * volume_);
		
		sides_ = n;
	}

	Polygon::~Polygon() {
		delete vertexes_;
		delete normals_;
	}

	int Polygon::get_sides() {
		return sides_;
	}

	Vec2D *Polygon::get_vertexes() {
		return vertexes_;
	}

	Vec2D *Polygon::get_normals() {
		return normals_;
	}

	float Polygon::get_inertia(float density) {
		float I = 0.0;

		for(int i = 0; i < sides_; i++) {
			Vec2D a = vertexes_[i] - centroid_;
			Vec2D b = vertexes_[(i+1)%sides_] - centroid_;
			
			float D = a.cross(b);
			float x2 = a.x*a.x + b.x*a.x + b.x*b.x;
			float y2 = a.y*a.y + b.y*a.y + b.y*b.y;

			I += D * (x2 + y2);
		}

		return I * density / 12.0;
	}

	bool Polygon::is_in_bounds(Vec2D local) {
		// Winding number algorithm
		int wn = 0;
		for(int i = 0; i < sides_; i++) {
			Vec2D a = vertexes_[i];
			Vec2D b = vertexes_[(i+1)%sides_];

			float det = ((b.x - a.x) * (local.y - a.y) 
					   - (b.y - a.y) * (local.x - a.x));
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
}