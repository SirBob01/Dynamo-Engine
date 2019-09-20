#ifndef GAME_PHYSICS_SHAPE
#define GAME_PHYSICS_SHAPE

#include "vec2d.h"

typedef enum {
	PHYSICS_SHAPE_POLYGON,
	PHYSICS_SHAPE_CIRCLE,
	PHYSICS_SHAPE_EDGE,
	PHYSICS_SHAPE_COUNT = 3,
} PHYSICS_SHAPE_TYPE;

// All points passed here by the Body should be in local coordinates
class PhysicsShape {
protected:
	PHYSICS_SHAPE_TYPE type;
	Vec2D centroid;
	float volume;

public:
	PhysicsShape(PHYSICS_SHAPE_TYPE t);

	PHYSICS_SHAPE_TYPE get_type();

	Vec2D get_centroid();

	float get_volume();

	virtual float get_inertia(float density);

	virtual bool is_in_bounds(Vec2D local);
};


#endif