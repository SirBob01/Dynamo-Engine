#include "../src/Dynamo.h"
#include <iostream>

// Define the components
// Use inheritance for components of similar type
struct Position : Dynamo::Vec2D {
    Position(float x, float y) 
    : Dynamo::Vec2D {x, y} {};
};

struct Velocity : Dynamo::Vec2D {
    Velocity(float dx, float dy) 
    : Dynamo::Vec2D {dx, dy} {};
};

// Define the systems
void physics_system(Dynamo::ECS &world, Dynamo::Vec2D logic_dim, unsigned dt) {
    world.each_group<Position, Velocity, float>(
        [dt, logic_dim](auto &entity, auto &pos, auto &vel, auto &r) {
            if(pos.x + r > logic_dim.x) {
                pos.x = logic_dim.x - r;
                vel.x *= -1;
            }
            if(pos.x - r < 0) {
                pos.x = r;
                vel.x *= -1;
            }
            if(pos.y + r > logic_dim.y) {
                pos.y = logic_dim.y - r;
                vel.y *= -1;
            }
            if(pos.y - r < 0) {
                pos.y = r;
                vel.y *= -1;
            }

            pos.x += vel.x * dt;
            pos.y += vel.y * dt;
    });
}

void draw_system(Dynamo::ECS &world, Dynamo::Renderer &renderer) {
    world.each_group<Position, float>(
        [&renderer](auto &entity, auto &pos, auto &r) {
            renderer.draw_rect(nullptr, {pos, {2*r, 2*r}}, {255, 0, 0}, true);
    });
}

// The initial scene
class Game : public Dynamo::Scene {
    Dynamo::ECS world;
    std::vector<Dynamo::Entity> entities;

public:
    void spawn() {
        Dynamo::Entity ball = world.create_entity();
        world.assign<Position>(ball, 500.0f, 300.0f);
        world.assign<Velocity>(ball, 
            Dynamo::Random::range(-1.0, 1.0), 
            Dynamo::Random::range(-1.0, 1.0)
        );
        world.assign<float>(ball, 5.0f);

        entities.push_back(ball);
    };

    void load(Dynamo::Core &core) {
        for(int i = 0; i < 1000; i++) {
            spawn();
        }
        std::cout << "Entity count: " << entities.size() << "\n";
    };

    void update(Dynamo::Core &core) {
        if(core.inputs.get_state_raw(Dynamo::Input::E)) {
            spawn();
            std::cout << "Entity count: " << entities.size() << "\n";
        }

        if(core.inputs.get_state_raw(Dynamo::Input::R) && entities.size()) {
            world.destroy_entity(entities.back());
            entities.pop_back();
            std::cout << "Entity count: " << entities.size() << "\n";
        }

        if(core.inputs.get_pressed_raw(Dynamo::Input::T)) {
            world.clear_all();
            entities.clear();
            std::cout << "Entity count: " << entities.size() << "\n";
        }

        physics_system(
            world,
            core.display.get_dimensions(), 
            core.clock.get_delta()
        );
        core.display.set_title("ECS Test - FPS: " + std::to_string(
                int(core.clock.get_fps())
            )
        );
    };

    void draw(Dynamo::Renderer &renderer) {
        renderer.set_fill({255, 255, 255});
        draw_system(world, renderer);
    };
};

// Entry point
int main(int argv, char **args) {
    Dynamo::Engine engine("ECS Test", 1000, 600);
    engine.push_scene<Game>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}