#include "body.h"

Physics::Body::Body(Physics::BodyDef def) {
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

Physics::Body::~Body() {
	Fixture *f = fixtures;
	Fixture *next;
	while(f != nullptr) {
		next = f->next;
		delete f;
		f = next;
	}
	fixtures = nullptr;
}

Physics::BODY_TYPE Physics::Body::get_type() {
	return type;
}

Physics::Fixture *Physics::Body::get_fixtures() {
	return fixtures;
}

Physics::Vec2D Physics::Body::get_pos() {
	return offset;
}

Physics::Vec2D Physics::Body::get_vel() {
	return vel;
}

float Physics::Body::get_angle() {
	return angle;
}

float Physics::Body::get_angular_vel() {
	return angular_vel;
}

float Physics::Body::get_inertia() {
	return inertia;
}

float Physics::Body::get_mass() {
	return net_mass;
}

void Physics::Body::add_fixture(Physics::Fixture f) {
	Fixture *fixture = new Fixture();
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
		Fixture *current = fixtures;
		while(current->next != nullptr) {
			current = current->next;
		}
		current->next = fixture;
	}
}

void Physics::Body::set_pos(Physics::Vec2D v) {
	offset = v;
}

void Physics::Body::set_vel(Physics::Vec2D v) {
	vel = v;
}

void Physics::Body::set_angle(float radians) {
	angle = wrap_val(radians, 0, 2*PI);

	float s = sin(angle);
	float c = cos(angle);

	rotform.a = c;
	rotform.b = -s;
	rotform.c = s;
	rotform.d = c;
}

void Physics::Body::set_angular_vel(float radps) {
	angular_vel = radps;
}

const Physics::Vec2D Physics::Body::convert_to_world(Physics::Vec2D local_point) {
	local_point -= center;
	return local_point.transform(rotform) + offset;
}

const Physics::Vec2D Physics::Body::convert_to_local(Physics::Vec2D world_point) {
	world_point -= offset;
	return world_point.transform(rotform.inverse()) + center;
}

bool Physics::Body::is_stationary() {
	// Body isn't moving in space
	bool linear = (vel.x == 0 && vel.y == 0);
	bool angular = (angular_vel == 0);
	return linear && angular;
}