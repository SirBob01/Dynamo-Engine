#ifndef DYNAMO_PHYSICS_WORLD_H_
#define DYNAMO_PHYSICS_WORLD_H_

#include <vector>

#include "body.h"
#include "narrow.h"
#include "broad.h"
#include "vec2d.h"

namespace Dynamo::Physics {
	class World {
		std::vector<Body *> bodies_;
		std::vector<Pair> pairs_;

		Vec2D world_forces_;

	public:
		World(Vec2D forces);

		~World();

		// Create a new body in the world
		Body *create_body(BodyDefinition body_def);

		void clear_bodies();

		std::vector<Body *> &get_bodies();

		void update_pairs();

		// Do a collision solve for a single pair
		void solve(Pair *pair);

		// A single update frame
		void update(float dt);
	};
}

#endif