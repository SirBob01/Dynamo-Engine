#ifndef GAME_PHYSICS_POLYGON
#define GAME_PHYSICS_POLYGON

#include "shape.h"
#include "vec2d.h"
#include "common.h"

class PhysicsPolygon : public PhysicsShape {
	Vec2D *vertexes;
	Vec2D *normals;

	int sides;

public:
	PhysicsPolygon(Vec2D verts[], int n);
	
	~PhysicsPolygon();

	int get_sides();

	Vec2D *get_vertexes();

	Vec2D *get_normals();

	float get_inertia(float density) override;

	bool is_in_bounds(Vec2D local) override;
};

#endif