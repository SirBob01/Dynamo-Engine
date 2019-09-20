#ifndef GAME_PHYSICS_WORLD
#define GAME_PHYSICS_WORLD

#include <vector>

#include "body.h"
#include "narrow.h"
#include "broad.h"
#include "vec2d.h"

class PhysicsWorld {
	std::vector<PhysicsBody *> bodies;
	std::vector<PhysicsPair> pairs;

	Vec2D world_forces;

public:
	PhysicsWorld(Vec2D forces);

	~PhysicsWorld();

	PhysicsBody *create_body(PhysicsBodyDef body_def);

	void clear_bodies();

	std::vector<PhysicsBody *> &get_bodies();

	void update_pairs();

	void solve(PhysicsPair *pair);

	void step(float dt);

	void update(float dt);
};

#endif