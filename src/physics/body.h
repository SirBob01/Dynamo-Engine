#ifndef GAME_PHYSICS_BODY
#define GAME_PHYSICS_BODY

#include "shape.h"
#include "vec2d.h"
#include "common.h"

typedef enum {
	PHYSICS_BODY_DYNAMIC,
	PHYSICS_BODY_STATIC,
	PHYSICS_BODY_COUNT = 2,
} PHYSICS_BODY_TYPE;

struct PhysicsFixture {
	// Describes physical properties of body
	PhysicsShape *shape;

	float density = 1.0; // rho = m / v
	float restitution = 1.0; // Bounce
	float friction = 0.0;
	float inertia = 0.0;

	PhysicsFixture *next = nullptr;
};

struct PhysicsBodyDef {
	PHYSICS_BODY_TYPE type;

	Vec2D pos;
	Vec2D vel;
	float angle;
	float angular_vel;
};

class PhysicsBody {
	PHYSICS_BODY_TYPE type;

	PhysicsFixture *fixtures;
	float net_mass;

	Vec2D center;
	Vec2D offset;
	Vec2D vel;

	float angle;
	float angular_vel;
	float inertia;

	Mat2D rotform;

public:
	PhysicsBody(PhysicsBodyDef def);
	~PhysicsBody();

	PHYSICS_BODY_TYPE get_type();
	PhysicsFixture *get_fixtures();

	Vec2D get_center();
	Vec2D get_pos();
	Vec2D get_vel();

	float get_angle();
	float get_angular_vel();

	float get_inertia();
	float get_mass();

	// Allows for more complex figures
	// E.g. Multiple shapes can simulate concave/rounded polygons
	void add_fixture(PhysicsFixture f);

	// Hard-set linear and angular components
	void set_pos(Vec2D v);
	void set_vel(Vec2D v);

	void set_angle(float radians);
	void set_angular_vel(float radps);

	const Vec2D convert_to_world(Vec2D local_point);
	const Vec2D convert_to_local(Vec2D world_point);

	bool is_stationary();
};

#endif