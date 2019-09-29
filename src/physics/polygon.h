#ifndef DYNAMO_PHYSICS_POLYGON_H_
#define DYNAMO_PHYSICS_POLYGON_H_

#include "shape.h"
#include "vec2d.h"
#include "common.h"

namespace Physics {
	class Polygon : public Shape {
		Vec2D *vertexes;
		Vec2D *normals;

		int sides;

	public:
		Polygon(Vec2D verts[], int n);
		
		~Polygon();

		int get_sides();

		Vec2D *get_vertexes();

		Vec2D *get_normals();

		float get_inertia(float density) override;

		bool is_in_bounds(Vec2D local) override;
	};
}

#endif