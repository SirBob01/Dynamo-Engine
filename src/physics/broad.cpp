#include "broad.h"

bool PhysicsProjection::is_overlapping(const PhysicsProjection &b) {
	return (min <= b.max && b.min <= max);
}

float PhysicsProjection::get_range() {
	return Physics_abs(max - min);
}

float PhysicsAABB::get_width() {
	return x.get_range();
}

float PhysicsAABB::get_height() {
	return y.get_range();
}

float PhysicsAABB::get_volume() {
	return get_width() * get_height();
}

void PhysicsAABB::add_volume(const PhysicsAABB &b) {
	x.min = Physics_min(x.min, b.x.min);
	x.max = Physics_max(x.max, b.x.max);

	y.min = Physics_min(y.min, b.y.min);
	y.max = Physics_max(y.max, b.y.max);
}

bool PhysicsAABB::is_colliding(const PhysicsAABB &b) {
	bool hor = x.is_overlapping(b.x);
	bool ver = y.is_overlapping(b.y);
	return hor && ver;
}

PhysicsAABB Physics_get_aabb_polygon(PhysicsPolygon *poly, PhysicsBody *body) {
	// Transform all the vertexes to world coordinates
	// Check the (min, max) of x and y values
	Vec2D min = body->convert_to_world(poly->get_centroid());
	Vec2D max = min;

	for(int i = 0; i < poly->get_sides(); i++) {
		Vec2D vertex = body->convert_to_world(poly->get_vertexes()[i]);			
		
		max.x = Physics_max(max.x, vertex.x);
		max.y = Physics_max(max.y, vertex.y);

		min.x = Physics_min(min.x, vertex.x);
		min.y = Physics_min(min.y, vertex.y);
	}
	return {{min.x, max.x},
			{min.y, max.y}};
}

PhysicsAABB Physics_get_aabb_circle(PhysicsCircle *circle, PhysicsBody *body) {
	// Transform center to world coordinates
	// min_vector = center - {radius, radius}
	// max_vector = center + {radius, radius}
	Vec2D min = body->convert_to_world(circle->get_centroid());
	Vec2D max = min;

	Vec2D center = body->convert_to_world(circle->get_centroid());
	float r = circle->get_radius();

	max.x = Physics_max(max.x, center.x + r);
	max.y = Physics_max(max.y, center.y + r);

	min.x = Physics_min(min.x, center.x - r);
	min.y = Physics_min(min.y, center.y - r);

	return {{min.x, max.x},
			{min.y, max.y}};
}

PhysicsAABB Physics_get_aabb_edge(PhysicsEdge *edge, PhysicsBody *body) {
	// TODO: Implement edges once done with polygons and circles
}

PhysicsAABB Physics_get_aabb(PhysicsBody *body) {
	Vec2D body_pos = body->get_pos();
	PhysicsAABB total_volume = {{body_pos.x, body_pos.x},
								{body_pos.y, body_pos.y}};

	for(PhysicsFixture *f = body->get_fixtures(); f; f = f->next) {
		PhysicsAABB sub_volume;
		switch(f->shape->get_type()) {
			case PHYSICS_SHAPE_POLYGON:
				sub_volume = Physics_get_aabb_polygon((PhysicsPolygon *)f->shape, body);
				break;
			case PHYSICS_SHAPE_CIRCLE:
				sub_volume = Physics_get_aabb_circle((PhysicsCircle *)f->shape, body);
				break;
			case PHYSICS_SHAPE_EDGE:
				sub_volume = Physics_get_aabb_edge((PhysicsEdge *)f->shape, body);
				break;
		}
		total_volume.add_volume(sub_volume);
	}
	return total_volume;
}