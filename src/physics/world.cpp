#include "world.h"

namespace Dynamo::Physics {
    World::World(Vec2D forces) {
        world_forces_ = forces;
    }

    World::~World() {
        clear_bodies();
    }

    Body *World::create_body(BodyDefinition body_def) {
        Body *b = new Body(body_def);
        bodies_.push_back(b);

        return b;
    }

    void World::clear_bodies() {
        // Clear all bodies
        for(auto &b : bodies_) {
            delete b;
        }
        bodies_.clear();
    }

    std::vector<Body *> &World::get_bodies() {
        return bodies_;
    }

    void World::update_pairs() {
        // TODO: Implement dynamic bounding volume tree for broad phase
        // Naive broad phase O(n^2) :(
        pairs_.clear();
        for(int i = 0; i < bodies_.size() - 1; i++) {
            for(int j = i + 1; j < bodies_.size(); j++) {
                Body *b1 = bodies_[i];
                Body *b2 = bodies_[j];

                AABB aabb1 = get_aabb(b1);
                AABB aabb2 = get_aabb(b2);
                if(aabb1.is_colliding(aabb2)) {
                    pairs_.push_back({b1, b2});
                }
            }
        }
    }

    void World::solve(Pair *pair) {
        // Resolve pair collisions
        for(Fixture *f1 = pair->a->get_fixtures(); f1; f1 = f1->next) {
            for(Fixture *f2 = pair->b->get_fixtures(); f2; f2 = f2->next) {
                Manifold m = colliding(pair, f1, f2);
                resolve_impulse(&m);
            }
        }
    }

    void World::update(float dt) {
        update_pairs();

        for(auto &pair : pairs_) {
            solve(&pair);
        }

        // Update all the bodies
        for(auto &b : bodies_) {
            Vec2D pos = b->get_pos();
            Vec2D vel = b->get_vel();

            float angle = b->get_angle();
            float angular_vel = b->get_angular_vel();

            if(b->get_type() == BODY_DYNAMIC) {
                // Acceleration = force / net_mass of body
                vel += world_forces_ / b->get_mass() * dt;
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
}