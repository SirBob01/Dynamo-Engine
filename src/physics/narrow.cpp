#include "narrow.h"

PhysicsProjection Physics_project_polygon(PhysicsPolygon *poly, Vec2D axis, PhysicsBody *body) {
	float p_max = axis * body->convert_to_world(poly->get_vertexes()[0]);
	float p_min = p_max;

	for(int i = 0; i < poly->get_sides(); i++) {
		float d = body->convert_to_world(poly->get_vertexes()[i]) * axis;
		if(d < p_min) {
			p_min = d;
		}
		else if(d > p_max) {
			p_max = d;
		}
	}

	return {p_min, p_max};
}

PhysicsProjection Physics_project_circle(PhysicsCircle *circle, Vec2D axis, PhysicsBody *body) {
	Vec2D world_center = body->convert_to_world(circle->get_centroid());
	float r = circle->get_radius();

	float a = axis * (world_center + (axis * r));
	float b = axis * (world_center - (axis * r));

	if(b > a) {
		return {a, b};
	}
	else {
		return {b, a};
	}
}

PhysicsProjection Physics_project_edge(PhysicsEdge *edge, Vec2D axis, PhysicsBody *body) {
	// TODO: Implement edges once done with polygons and circles
}

// Manifold generation
bool Physics_colliding_polygons(PhysicsPolygon *a, PhysicsPolygon *b, PhysicsPair *pair, PhysicsManifold *m) {
	return false;
}

bool Physics_colliding_circles(PhysicsCircle *a, PhysicsCircle *b, PhysicsPair *pair, PhysicsManifold *m) {
	Vec2D a_pos = pair->a->convert_to_world(a->get_centroid());
	Vec2D b_pos = pair->b->convert_to_world(b->get_centroid());

	Vec2D diff = b_pos - a_pos;
	float l_sqr = diff.length_squared();
	float r_sum = a->get_radius() + b->get_radius();

	if(l_sqr >= r_sum * r_sum) {
		return false;
	}
	else {
		float length = sqrt(l_sqr);
		if(m != nullptr) {
			if(length != 0) {
				Vec2D normal = diff / length;

				m->normal = normal;
				m->contact = a_pos + (normal * a->get_radius());
				m->penetration = r_sum - length;
			}
			else {
				m->normal = {1, 0};
				m->contact = a_pos;
				m->penetration = a->get_radius();
			}
		}
		return true;
	}
}

bool Physics_colliding_polygon_circle(PhysicsPolygon *p, PhysicsCircle *c, PhysicsPair *pair, PhysicsManifold *m) {
	return false;
}

PhysicsManifold Physics_colliding(PhysicsPair *pair, PhysicsFixture *a, PhysicsFixture *b) {
	PhysicsManifold m;
	m.pair = pair;
	m.a = a;
	m.b = b;

	// Call suitable collision functions
	PHYSICS_SHAPE_TYPE a_type = a->shape->get_type();
	PHYSICS_SHAPE_TYPE b_type = b->shape->get_type();

	if(a_type == PHYSICS_SHAPE_POLYGON && b_type == PHYSICS_SHAPE_POLYGON) {
		// Polygon - polygon
		Physics_colliding_polygons((PhysicsPolygon *)a->shape, (PhysicsPolygon *)b->shape, pair, &m);
	}
	else if(a_type == PHYSICS_SHAPE_CIRCLE && b_type == PHYSICS_SHAPE_CIRCLE) {
		// Circle - circle
		Physics_colliding_circles((PhysicsCircle *)a->shape, (PhysicsCircle *)b->shape, pair, &m);
	}
	else if(a_type == PHYSICS_SHAPE_POLYGON && b_type == PHYSICS_SHAPE_CIRCLE) {
		// Polygon - circle
		Physics_colliding_polygon_circle((PhysicsPolygon *)a->shape, (PhysicsCircle *)b->shape, pair, &m);
	}
	else if(a_type == PHYSICS_SHAPE_CIRCLE && b_type == PHYSICS_SHAPE_POLYGON) {
		// Circle - polygon
		Physics_colliding_polygon_circle((PhysicsPolygon *)b->shape, (PhysicsCircle *)a->shape, pair, &m);
	}

	return m;
}


void Physics_resolve_impulse(PhysicsManifold *m) {
	// This is the bulk of the actual physics resolution code
	// Restitution and friction coefficients
	float e = Physics_min(m->a->restitution, m->b->restitution);
	float f = Physics_min(1.0, m->a->friction + m->b->friction);

	// Inverse masses
	float invm_a = 1.0 / m->pair->a->get_mass();
	float invm_b = 1.0 / m->pair->b->get_mass();

	// Inverse inertias
	float invI_a = 1.0 / m->pair->a->get_inertia();
	float invI_b = 1.0 / m->pair->a->get_inertia();

	// Linear motion
	Vec2D a_lin = m->pair->a->get_vel();
	Vec2D b_lin = m->pair->b->get_vel();

	// Angular motion
	float a_ang = m->pair->a->get_angular_vel();
	float b_ang = m->pair->b->get_angular_vel();

	Vec2D r_a = (m->contact - m->pair->a->get_pos());
	Vec2D r_b = (m->contact - m->pair->b->get_pos());
 
	Vec2D rot_a = m->normal * (invI_a * r_a.cross(m->normal));
	Vec2D rot_b = m->normal * (invI_b * r_b.cross(m->normal));

	// Initial tangential velocities
	// Proportional to radius-normal perpendicularity
	Vec2D a_tan = m->normal * (r_a.cross(m->normal)) * a_ang;
	Vec2D b_tan = m->normal * (r_b.cross(m->normal)) * b_ang;

	// Total initial = V + omega * r
	Vec2D a_ivel = a_lin + a_tan;
	Vec2D b_ivel = b_lin + b_tan;

	// Friction vector
	// Relative velocity projected onto tangent of collision
	Vec2D f_vec = m->normal.left_normal() * ((b_ivel - a_ivel) * m->normal.left_normal());

	// Impulse vector
	Vec2D impulse = m->normal - f_vec * f;
	impulse *= -(1.0 + e)*((b_ivel - a_ivel) * m->normal);
	impulse /= invm_a + invm_b + rot_a.cross(r_a) + rot_b.cross(r_b);

	// Correct positions
	float allowance = 0.01;
	float corr_coeff = Physics_max(m->penetration - allowance, 0.0) / (invm_a + invm_b);

	// Update final velocities
	if(m->pair->a->get_type() == PHYSICS_BODY_DYNAMIC) {
		Vec2D a_pos = m->pair->a->get_pos();

		m->pair->a->set_pos(a_pos - m->normal * corr_coeff * invm_a);
		m->pair->a->set_vel(a_lin - (impulse * invm_a));
		m->pair->a->set_angular_vel(a_ang + (invI_a * r_a.cross(impulse)));
	}

	if(m->pair->b->get_type() == PHYSICS_BODY_DYNAMIC) {
		float b_ang = m->pair->b->get_angular_vel();
		Vec2D b_pos = m->pair->b->get_pos();

		m->pair->b->set_pos(b_pos + m->normal * corr_coeff * invm_b);
		m->pair->b->set_vel(b_lin + (impulse * invm_b));
		m->pair->b->set_angular_vel(b_ang + (invI_b * r_b.cross(impulse)));
	}
}