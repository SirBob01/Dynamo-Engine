#include "world.h"

PhysicsWorld::PhysicsWorld(Vec2D forces) {
	world_forces = forces;
}

PhysicsWorld::~PhysicsWorld() {
	clear_bodies();
}

PhysicsBody *PhysicsWorld::create_body(PhysicsBodyDef body_def) {
	PhysicsBody *b = new PhysicsBody(body_def);
	bodies.push_back(b);

	return b;
}

void PhysicsWorld::clear_bodies() {
	// Clear all bodies
	for(auto &b : bodies) {
		delete b;
	}
	bodies.clear();
}

std::vector<PhysicsBody *> &PhysicsWorld::get_bodies() {
	return bodies;
}

void PhysicsWorld::update_pairs() {
	// TODO: Implement dynamic bounding volume tree for broad phase
	// Naive broad phase O(n^2) :(
	pairs.clear();
	for(int i = 0; i < bodies.size() - 1; i++) {
		for(int j = i + 1; j < bodies.size(); j++) {
			PhysicsBody *b1 = bodies[i];
			PhysicsBody *b2 = bodies[j];

			PhysicsAABB aabb1 = Physics_get_aabb(b1);
			PhysicsAABB aabb2 = Physics_get_aabb(b2);
			if(aabb1.is_colliding(aabb2)) {
				pairs.push_back({b1, b2});
			}
		}
	}
}

void PhysicsWorld::solve(PhysicsPair *pair) {
	// Resolve pair collisions
	for(PhysicsFixture *f1 = pair->a->get_fixtures(); f1; f1 = f1->next) {
		for(PhysicsFixture *f2 = pair->b->get_fixtures(); f2; f2 = f2->next) {				
			PhysicsManifold m = Physics_colliding(pair, f1, f2);
			Physics_resolve_impulse(&m);
		}
	}
}

void PhysicsWorld::step(float dt) {
	for(auto &pair : pairs) {
		solve(&pair);
	}

	// Update all the bodies
	for(auto &b : bodies) {
		Vec2D pos = b->get_pos();
		Vec2D vel = b->get_vel();

		float angle = b->get_angle();
		float angular_vel = b->get_angular_vel();

		if(b->get_type() == PHYSICS_BODY_DYNAMIC) {
			// Acceleration = force / net_mass of body
			vel += world_forces / b->get_mass() * dt;
		}

		pos += vel * dt;
		angle += angular_vel * dt;

		// Overwrite
		b->set_vel(vel);
		b->set_pos(pos);

		if(angle != b->get_angle()) {
			// Minimize sin, cos calculation in rotational transform
			b->set_angle(angle);
		}
	}
}

void PhysicsWorld::update(float dt) {
	update_pairs();
	step(dt);
}