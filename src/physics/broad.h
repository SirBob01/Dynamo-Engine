#ifndef DYNAMO_PHYSICS_BROAD_H_
#define DYNAMO_PHYSICS_BROAD_H_

#include <stack>

#include "body.h"
#include "polygon.h"
#include "circle.h"
#include "edge.h"
#include "common.h"
#include "vec2d.h"

namespace Physics {
	struct Pair {
		Body *a;
		Body *b;
	};

	struct Projection {
		float min, max;

		float get_range();
		
		bool is_overlapping(const Projection &b);
	};

	// TODO: Implement bounding volume hierarchy
	struct AABB {
		Projection x;
		Projection y;

		AABB *left = nullptr;
		AABB *right = nullptr;

		float get_width();
		float get_height();
		float get_volume();

		void add_volume(const AABB &b);

		bool is_colliding(const AABB &b);
	};

	AABB get_aabb_polygon(Polygon *poly, Body *body);

	AABB get_aabb_circle(Circle *circle, Body *body);

	AABB get_aabb_edge(Edge *edge, Body *body);

	AABB get_aabb(Body *body);
}

#endif