#include "body.h"

namespace Dynamo::Physics {
	Body::Body(BodyDefinition def) {
		// Default values
		type_ = def.type;

		fixtures_ = nullptr;
		net_mass_ = 0.0;

		center_ = {0, 0};
		offset_ = def.pos;
		vel_ = def.vel;

		angular_vel_ = def.angular_vel;
		inertia_ = 0.0;

		rotform_ = {1, 0,
				   0, 1};

		set_angle(def.angle);
	}

	Body::~Body() {
		Fixture *f = fixtures_;
		Fixture *next;
		while(f != nullptr) {
			next = f->next;
			delete f;
			f = next;
		}
		fixtures_ = nullptr;
	}

	BODY_TYPE Body::get_type() {
		return type_;
	}

	Fixture *Body::get_fixtures() {
		return fixtures_;
	}

	Vec2D Body::get_pos() {
		return offset_;
	}

	Vec2D Body::get_vel() {
		return vel_;
	}

	float Body::get_angle() {
		return angle_;
	}

	float Body::get_angular_vel() {
		return angular_vel_;
	}

	float Body::get_inertia() {
		return inertia_;
	}

	float Body::get_mass() {
		return net_mass_;
	}

	void Body::add_fixture(Fixture fixture) {
		Fixture *new_f = new Fixture();
		new_f->shape = fixture.shape;
		new_f->density = fixture.density;
		new_f->restitution = fixture.restitution;
		new_f->friction = fixture.friction;

		// Update net mass and center of mass
		// center of mass = sum(centroid * m) / net_mass 
		center_ *= net_mass_;
		float m = new_f->density * new_f->shape->get_volume();
		center_ += new_f->shape->get_centroid() * m;
		net_mass_ += m;
		center_ /= net_mass_;

		// Update inertia of new_f
		new_f->inertia = new_f->shape->get_inertia(fixture.density);

		// Update inertia of body (parallel axis theorem)
		Vec2D d = center_ - new_f->shape->get_centroid();
		inertia_ = new_f->inertia - m*d.length_squared();

		if(fixtures_ == nullptr) {
			fixtures_ = new_f;
		}
		else {
			Fixture *current = fixtures_;
			while(current->next != nullptr) {
				current = current->next;
			}
			current->next = new_f;
		}
	}

	void Body::set_pos(Vec2D pos) {
		offset_ = pos;
	}

	void Body::set_vel(Vec2D vel) {
		vel_ = vel;
	}

	void Body::set_angle(float radians) {
		angle_ = wrap_val(radians, 0, 2*PI);

		float s = sin(angle_);
		float c = cos(angle_);

		rotform_.a = c;
		rotform_.b = -s;
		rotform_.c = s;
		rotform_.d = c;
	}

	void Body::set_angular_vel(float rps) {
		angular_vel_ = rps;
	}

	const Vec2D Body::convert_to_world(Vec2D local) {
		local -= center_;
		return local.transform(rotform_) + offset_;
	}

	const Vec2D Body::convert_to_local(Vec2D world) {
		world -= offset_;
		return world.transform(rotform_.inverse()) + center_;
	}

	bool Body::is_stationary() {
		// Body isn't moving in space
		bool linear = (vel_.x == 0 && vel_.y == 0);
		bool angular = (angular_vel_ == 0);
		return linear && angular;
	}
}