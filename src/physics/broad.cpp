#include "broad.h"

bool Physics::Projection::is_overlapping(const Physics::Projection &b) {
	return (min <= b.max && b.min <= max);
}

float Physics::Projection::get_range() {
	return abs(max - min);
}

float Physics::AABB::get_width() {
	return x.get_range();
}

float Physics::AABB::get_height() {
	return y.get_range();
}

float Physics::AABB::get_volume() {
	return get_width() * get_height();
}

void Physics::AABB::add_volume(const Physics::AABB &b) {
	x.min = min(x.min, b.x.min);
	x.max = max(x.max, b.x.max);

	y.min = min(y.min, b.y.min);
	y.max = max(y.max, b.y.max);
}

bool Physics::AABB::is_colliding(const Physics::AABB &b) {
	bool hor = x.is_overlapping(b.x);
	bool ver = y.is_overlapping(b.y);
	return hor && ver;
}

Physics::AABB Physics::get_aabb_polygon(Physics::Polygon *poly, Physics::Body *body) {
	// Transform all the vertexes to world coordinates
	// Check the (min, max) of x and y values
	Vec2D min_v = body->convert_to_world(poly->get_centroid());
	Vec2D max_v = min_v;

	for(int i = 0; i < poly->get_sides(); i++) {
		Vec2D vertex = body->convert_to_world(poly->get_vertexes()[i]);			
		
		max_v.x = max(max_v.x, vertex.x);
		max_v.y = max(max_v.y, vertex.y);

		min_v.x = min(min_v.x, vertex.x);
		min_v.y = min(min_v.y, vertex.y);
	}
	return {{min_v.x, max_v.x},
			{min_v.y, max_v.y}};
}

Physics::AABB Physics::get_aabb_circle(Physics::Circle *circle, Physics::Body *body) {
	// Transform center to world coordinates
	// min_vector = center - {radius, radius}
	// max_vector = center + {radius, radius}
	Vec2D min_v = body->convert_to_world(circle->get_centroid());
	Vec2D max_v = min_v;

	Vec2D center = body->convert_to_world(circle->get_centroid());
	float r = circle->get_radius();

	max_v.x = max(max_v.x, center.x + r);
	max_v.y = max(max_v.y, center.y + r);

	min_v.x = min(min_v.x, center.x - r);
	min_v.y = min(min_v.y, center.y - r);

	return {{min_v.x, max_v.x},
			{min_v.y, max_v.y}};
}

Physics::AABB Physics::get_aabb_edge(Physics::Edge *edge, Physics::Body *body) {
	// TODO: Implement edges once done with polygons and circles
}

Physics::AABB Physics::get_aabb(Physics::Body *body) {
	Vec2D body_pos = body->get_pos();
	AABB total_volume = {{body_pos.x, body_pos.x},
								{body_pos.y, body_pos.y}};

	for(Fixture *f = body->get_fixtures(); f; f = f->next) {
		AABB sub_volume;
		switch(f->shape->get_type()) {
			case SHAPE_POLYGON:
				sub_volume = get_aabb_polygon((Polygon *)f->shape, body);
				break;
			case SHAPE_CIRCLE:
				sub_volume = get_aabb_circle((Circle *)f->shape, body);
				break;
			case SHAPE_EDGE:
				sub_volume = get_aabb_edge((Edge *)f->shape, body);
				break;
		}
		total_volume.add_volume(sub_volume);
	}
	return total_volume;
}