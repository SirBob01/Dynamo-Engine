#ifndef GAME_PHYSICS_BROAD
#define GAME_PHYSICS_BROAD

#include <stack>

#include "body.h"
#include "polygon.h"
#include "circle.h"
#include "edge.h"
#include "common.h"
#include "vec2d.h"

struct PhysicsPair {
	PhysicsBody *a;
	PhysicsBody *b;
};

struct PhysicsProjection {
	float min, max;

	float get_range();
	
	bool is_overlapping(const PhysicsProjection &b);
};

// TODO: Implement bounding volume hierarchy
struct PhysicsAABB {
	PhysicsProjection x;
	PhysicsProjection y;

	PhysicsAABB *left = nullptr;
	PhysicsAABB *right = nullptr;

	float get_width();
	float get_height();
	float get_volume();

	void add_volume(const PhysicsAABB &b);

	bool is_colliding(const PhysicsAABB &b);
};

PhysicsAABB Physics_get_aabb_polygon(PhysicsPolygon *poly, PhysicsBody *body);

PhysicsAABB Physics_get_aabb_circle(PhysicsCircle *circle, PhysicsBody *body);

PhysicsAABB Physics_get_aabb_edge(PhysicsEdge *edge, PhysicsBody *body);

PhysicsAABB Physics_get_aabb(PhysicsBody *body);

#endif