#ifndef DYNAMO_PHYSICS_BODY_H_
#define DYNAMO_PHYSICS_BODY_H_

#include "shape.h"
#include "vec2d.h"
#include "common.h"

namespace Dynamo::Physics {
	typedef enum {
		BODY_DYNAMIC,
		BODY_STATIC,
		BODY_COUNT = 2,
	} BODY_TYPE;

	struct Fixture {
		// Describes physical properties of body
		Shape *shape;

		float density = 1.0; // rho = m / v
		float restitution = 1.0; // Bounce
		float friction = 0.0;
		float inertia = 0.0;

		Fixture *next = nullptr;
	};

	struct BodyDef {
		BODY_TYPE type;

		Vec2D pos;
		Vec2D vel;
		float angle;
		float angular_vel;
	};

	class Body {
		BODY_TYPE type;

		Fixture *fixtures;
		float net_mass;

		Vec2D center;
		Vec2D offset;
		Vec2D vel;

		float angle;
		float angular_vel;
		float inertia;

		Mat2D rotform;

	public:
		Body(BodyDef def);
		~Body();

		BODY_TYPE get_type();
		Fixture *get_fixtures();

		Vec2D get_center();
		Vec2D get_pos();
		Vec2D get_vel();

		float get_angle();
		float get_angular_vel();

		float get_inertia();
		float get_mass();

		// Allows for more complex figures
		// E.g. Multiple shapes can simulate concave/rounded polygons
		void add_fixture(Fixture f);

		// Hard-set linear and angular components
		void set_pos(Vec2D v);
		void set_vel(Vec2D v);

		void set_angle(float radians);
		void set_angular_vel(float radps);

		const Vec2D convert_to_world(Vec2D local_point);
		const Vec2D convert_to_local(Vec2D world_point);

		bool is_stationary();
	};
}

#endif