#ifndef GAME_PHYSICS_NARROW
#define GAME_PHYSICS_NARROW

#include "body.h"
#include "polygon.h"
#include "circle.h"
#include "edge.h"
#include "broad.h"
#include "vec2d.h"

struct PhysicsManifold {
	// Participating bodies
	PhysicsPair *pair;
	
	// Relevant fixtures
	PhysicsFixture *a;
	PhysicsFixture *b;

	// Collision data
	Vec2D normal; // Normalized direction
	Vec2D contact;
	float penetration;
};

// Project shape onto axis for SAT algorithm
PhysicsProjection Physics_project_polygon(PhysicsPolygon *poly, Vec2D axis, PhysicsBody *body);

PhysicsProjection Physics_project_circle(PhysicsCircle *circle, Vec2D axis, PhysicsBody *body);

PhysicsProjection Physics_project_edge(PhysicsEdge *edge, Vec2D axis, PhysicsBody *body);

// Manifold generation
bool Physics_colliding_circles(PhysicsCircle *a, PhysicsCircle *b, PhysicsPair *pair, PhysicsManifold *m);

// Collision wrapper
PhysicsManifold Physics_colliding(PhysicsPair *pair, PhysicsFixture *a, PhysicsFixture *b);

// Impulse calculation
void Physics_resolve_impulse(PhysicsManifold *m);

#endif