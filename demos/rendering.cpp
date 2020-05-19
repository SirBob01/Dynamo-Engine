#include "../src/Dynamo.h"

class Game : public Dynamo::Scene {
    Dynamo::Sprite *blurry_circle;
    Dynamo::Sprite *smiley;
    Dynamo::Vec2D poly[5];

public:
    void load(Dynamo::Core &core) override;
    
    void unload(Dynamo::Core &core) override;
    
    void update(Dynamo::Core &core) override;

    void draw(Dynamo::Renderer &renderer) override;
};

// Definitions
void Game::load(Dynamo::Core &core) {
    // Dynamic texture generation test
    float r = 100.0;
    Dynamo::Color colors[(int)(r*2)][(int)(r*2)];
    Dynamo::Color base = {255, 0, 0, 255};

    for(float x = -r; x < r; x++) {
        for(float y = -r; y < r; y++) {
            Dynamo::Vec2D point = {x, y};
            float t = point.length_squared()/(r*r);
            Dynamo::Color color;
            if(t > 1.0) {
                color = {255, 0, 0, 0};
            }
            else {
                color = base.lerp({255, 0, 0, 0}, t);
            }
            color.a *= 0.75;
            colors[int(y+r)][int(x+r)] = color;
        }
    }
    blurry_circle = new Dynamo::Sprite(
        core.textures.generate_texture(
            "blurry circle", 
            {r*2, r*2}, 
            (Dynamo::Color *)colors
        )
    );
    smiley = new Dynamo::Sprite(
        core.textures.load_image("assets/textures/ball.png"),
        {50, 50}
    );

    poly[0] = {50, 10};
    poly[1] = {70, 60};
    poly[2] = {65, 20};
    poly[3] = {40, 70};
    poly[4] = {35, 30};
};

void Game::unload(Dynamo::Core &core) {
    delete blurry_circle;
    delete smiley;
}

void Game::update(Dynamo::Core &core) {
    smiley->animate(core.clock.get_delta(), 12, true);
}

void Game::draw(Dynamo::Renderer &renderer) {
    renderer.set_fill({255, 255, 255});

    renderer.draw_polygon(nullptr, poly, 5, {10, 10, 255}, true);
    renderer.draw_sprite(nullptr, smiley, {100, 100});
    
    // Create an offset polygon to test drawing filled shapes 
    Dynamo::Vec2D other_poly[5];
    for(int i = 0; i < 5; i++) {
        other_poly[i] = poly[i];
        other_poly[i].x += 50;
    }
    renderer.draw_polygon(nullptr, other_poly, 5, {10, 10, 255}, false);
    renderer.draw_circle(nullptr, {500, 300}, 250, {255, 0, 255}, false);
    renderer.draw_circle(nullptr, {500, 300}, 100, {255, 0, 255}, true);
    renderer.draw_sprite(nullptr, blurry_circle, {700, 300});
}

int main(int argv, char **args) {
    // Entry point function
    Dynamo::Engine engine("Rendering Test", 1000, 600);
    engine.push_scene<Game>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}