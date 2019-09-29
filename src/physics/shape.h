#ifndef DYNAMO_PHYSICS_SHAPE_H_
#define DYNAMO_PHYSICS_SHAPE_H_

#include "vec2d.h"

namespace Physics {
	typedef enum {
		SHAPE_POLYGON,
		SHAPE_CIRCLE,
		SHAPE_EDGE,
		SHAPE_COUNT = 3,
	} SHAPE_TYPE;

	// All points passed here by the Body should be in local coordinates
	class Shape {
	protected:
		SHAPE_TYPE type;
		Vec2D centroid;
		float volume;

	public:
		Shape(SHAPE_TYPE t);

		SHAPE_TYPE get_type();

		Vec2D get_centroid();

		float get_volume();

		virtual float get_inertia(float density);

		virtual bool is_in_bounds(Vec2D local);
	};
}

#endif