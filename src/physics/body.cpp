#include "body.h"

PhysicsBody::PhysicsBody(PhysicsBodyDef def) {
	// Default values
	type = def.type;

	fixtures = nullptr;
	net_mass = 0.0;

	center = {0, 0};
	offset = def.pos;
	vel = def.vel;

	angular_vel = def.angular_vel;
	inertia = 0.0;

	rotform = {1, 0,
			   0, 1};

	set_angle(def.angle);
}

PhysicsBody::~PhysicsBody() {
	PhysicsFixture *f = fixtures;
	PhysicsFixture *next;
	while(f != nullptr) {
		next = f->next;
		delete f;
		f = next;
	}
	fixtures = nullptr;
}

PHYSICS_BODY_TYPE PhysicsBody::get_type() {
	return type;
}

PhysicsFixture *PhysicsBody::get_fixtures() {
	return fixtures;
}

Vec2D PhysicsBody::get_pos() {
	return offset;
}

Vec2D PhysicsBody::get_vel() {
	return vel;
}

float PhysicsBody::get_angle() {
	return angle;
}

float PhysicsBody::get_angular_vel() {
	return angular_vel;
}

float PhysicsBody::get_inertia() {
	return inertia;
}

float PhysicsBody::get_mass() {
	return net_mass;
}

void PhysicsBody::add_fixture(PhysicsFixture f) {
	PhysicsFixture *fixture = new PhysicsFixture();
	fixture->shape = f.shape;
	fixture->density = f.density;
	fixture->restitution = f.restitution;
	fixture->friction = f.friction;

	// Update net mass and center of mass
	// center of mass = sum(centroid * m) / net_mass 
	center *= net_mass;
	float m = fixture->density * fixture->shape->get_volume();
	center += fixture->shape->get_centroid() * m;
	net_mass += m;
	center /= net_mass;

	// Update inertia of fixture
	fixture->inertia = f.shape->get_inertia(f.density);

	// Update inertia of body (parallel axis theorem)
	Vec2D d = center - fixture->shape->get_centroid();
	inertia = fixture->inertia - m*d.length_squared();

	if(fixtures == nullptr) {
		fixtures = fixture;
	}
	else {
		PhysicsFixture *current = fixtures;
		while(current->next != nullptr) {
			current = current->next;
		}
		current->next = fixture;
	}
}

void PhysicsBody::set_pos(Vec2D v) {
	offset = v;
}

void PhysicsBody::set_vel(Vec2D v) {
	vel = v;
}

void PhysicsBody::set_angle(float radians) {
	angle = Physics_wrap_val(radians, 0, 2*PI);

	float s = sin(angle);
	float c = cos(angle);

	rotform.a = c;
	rotform.b = -s;
	rotform.c = s;
	rotform.d = c;
}

void PhysicsBody::set_angular_vel(float radps) {
	angular_vel = radps;
}

const Vec2D PhysicsBody::convert_to_world(Vec2D local_point) {
	local_point -= center;
	return local_point.transform(rotform) + offset;
}

const Vec2D PhysicsBody::convert_to_local(Vec2D world_point) {
	world_point -= offset;
	return world_point.transform(rotform.inverse()) + center;
}

bool PhysicsBody::is_stationary() {
	// Body isn't moving in space
	bool linear = (vel.x == 0 && vel.y == 0);
	bool angular = (angular_vel == 0);
	return linear && angular;
}