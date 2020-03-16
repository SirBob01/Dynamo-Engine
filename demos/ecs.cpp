#include "../src/Dynamo.h"

// Define the components
struct Drawable {
    Dynamo::Sprite *sprite;

    ~Drawable() {
        delete sprite;
    };
};

struct Velocity {
    Dynamo::Vec2D vel;
};

struct Controllable {
    Dynamo::Inputs *inputs;
};

// Define the systems
void control_system(Dynamo::ECS *world) {
    for(auto &entity : world->get_group<Velocity, Controllable>()) {
        Velocity *vel = world->get_component<Velocity>(entity);
        Controllable *controller = world->get_component<Controllable>(entity);
        
        Dynamo::Inputs *inputs = controller->inputs;
        if(inputs->get_pressed_raw(Dynamo::INPUT_W)) {
            vel->vel.y = -0.5;
        }
        if(inputs->get_pressed_raw(Dynamo::INPUT_S)) {
            vel->vel.y = 0.5;
        }
        if(inputs->get_pressed_raw(Dynamo::INPUT_A)) {
            vel->vel.x = -0.5;
        }
        if(inputs->get_pressed_raw(Dynamo::INPUT_D)) {
            vel->vel.x = 0.5;
        }

        if(inputs->get_released_raw(Dynamo::INPUT_W) && vel->vel.y < 0) {
            vel->vel.y = 0;
        }
        if(inputs->get_released_raw(Dynamo::INPUT_S) && vel->vel.y > 0) {
            vel->vel.y = 0;
        }
        if(inputs->get_released_raw(Dynamo::INPUT_A) && vel->vel.x < 0) {
            vel->vel.x = 0;
        }
        if(inputs->get_released_raw(Dynamo::INPUT_D) && vel->vel.x > 0) {
            vel->vel.x = 0;
        }
    }
}

void move_system(unsigned dt, Dynamo::ECS *world) {
    for(auto &entity : world->get_group<Dynamo::Vec2D, Velocity>()) {
        Dynamo::Vec2D *pos = world->get_component<Dynamo::Vec2D>(entity);
        Velocity *vel = world->get_component<Velocity>(entity);
        *pos += vel->vel * dt;
    }
}

void draw_system(unsigned dt, Dynamo::ECS *world, Dynamo::Display *display) {
    for(auto &entity : world->get_group<Dynamo::Vec2D, Drawable>()) {
        Drawable *sprite = world->get_component<Drawable>(entity);
        Dynamo::Vec2D *pos = world->get_component<Dynamo::Vec2D>(entity);
        sprite->sprite->animate(dt, 24, true);

        display->draw_sprite(
            nullptr, 
            sprite->sprite,
            *(pos)
        );
    }
}

// Main game scene
class Game : public Dynamo::Scene {
    Dynamo::ECS *world;
    Dynamo::Entity player1, player2;

public:
    Game(Dynamo::Core modules) : Dynamo::Scene(modules) {};
    
    void on_entry() override {
        world = new Dynamo::ECS();

        player1 = world->create_entity();
        world->add_component<Dynamo::Vec2D>(player1, 500.0f, 500.0f);
        world->add_component<Drawable>(player1, 
            new Dynamo::Sprite(
                    textures_->load_image("assets/textures/player.png"), 
                    {64, 205}
                )
        );
        world->add_component<Velocity>(player1, 0.0f, 0.0f);
        world->add_component<Controllable>(player1, inputs_);

        player2 = world->create_entity();
        world->add_component<Dynamo::Vec2D>(player2, 800.0f, 100.0f);
        world->add_component<Drawable>(player2, 
            new Dynamo::Sprite(
                    textures_->load_image("assets/textures/player.png"), 
                    {64, 205}
                )
        );
        world->add_component<Velocity>(player2, -0.1f, 0.0f);
    };

    void on_exit() override {
        delete world;
    };

    void update(unsigned dt) override {
        if(inputs_->get_pressed_raw(Dynamo::INPUT_E)) {
            world->destroy_entity(player1);
        }

        if(inputs_->get_pressed_raw(Dynamo::INPUT_R)) {
            world->destroy_entity(player2);
        }

        if(inputs_->get_pressed_raw(Dynamo::INPUT_Y)) {
            world->clear<Dynamo::Vec2D>();
        }

        move_system(dt, world);
        control_system(world);
    };

    void draw() override {
        display_->set_fill({255, 255, 255});
        draw_system(clock_->get_delta(), world, display_);
    };
};

int main(int argv, char **args) {
    Dynamo::Engine game_engine("ECS Test", false, 1000, 600);
    game_engine.push_scene<Game>();
    game_engine.start();

    while(game_engine.is_running()) {
        game_engine.run();
    }

    game_engine.quit();
    return 0;
}