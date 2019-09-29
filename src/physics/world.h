#ifndef DYNAMO_PHYSICS_WORLD_H_
#define DYNAMO_PHYSICS_WORLD_H_

#include <vector>

#include "body.h"
#include "narrow.h"
#include "broad.h"
#include "vec2d.h"

namespace Dynamo::Physics {
	class World {
		std::vector<Body *> bodies;
		std::vector<Pair> pairs;

		Vec2D world_forces;

	public:
		World(Vec2D forces);

		~World();

		Body *create_body(BodyDef body_def);

		void clear_bodies();

		std::vector<Body *> &get_bodies();

		void update_pairs();

		void solve(Pair *pair);

		void step(float dt);

		void update(float dt);
	};
}

#endif