#include "../src/Dynamo.h"

class Game : public Dynamo::Scene {
    Dynamo::Sprite *blurry_circle;
    Dynamo::Vec2D poly[5];

public:
    Game(Dynamo::Core modules) : Dynamo::Scene(modules) {};
    
    void on_entry() override;
    void draw() override;
};

// Definitions
void Game::on_entry() {
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
        textures_->generate_texture(
            "blurry circle", 
            {r*2, r*2}, 
            (Dynamo::Color *)colors
        )
    );

    poly[0] = {50, 10};
    poly[1] = {70, 60};
    poly[2] = {65, 20};
    poly[3] = {40, 70};
    poly[4] = {35, 30};
};

void Game::draw() {
    display_->set_fill({255, 255, 255});

    display_->draw_polygon(nullptr, poly, 5, {10, 10, 255}, true);
    
    // Create an offset polygon to test drawing filled shapes 
    Dynamo::Vec2D other_poly[5];
    for(int i = 0; i < 5; i++) {
        other_poly[i] = poly[i];
        other_poly[i].x += 50;
    }
    display_->draw_polygon(nullptr, other_poly, 5, {10, 10, 255}, false);
    display_->draw_circle(nullptr, {500, 300}, 250, {255, 0, 255}, false);
    display_->draw_circle(nullptr, {500, 300}, 100, {255, 0, 255}, true);
    display_->draw_sprite(nullptr, blurry_circle, {700, 300});
}

int main(int argv, char **args) {
    // Entry point function
    Dynamo::Engine game_engine("Rendering Test", false, 1000, 600);
    game_engine.push_scene<Game>();
    game_engine.start();

    while(game_engine.is_running()) {
        game_engine.run();
    }

    game_engine.quit();
    return 0;
}