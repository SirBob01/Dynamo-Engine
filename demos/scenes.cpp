#include "../src/Dynamo.h"

// Recommended: Forward declare all scenes to prevent circular dependency bugs
class First : public Dynamo::Scene {
    Dynamo::Sprite *text;

public:
    First(Dynamo::Core modules) : Dynamo::Scene(modules) {};

    void on_entry() override;
    void on_exit() override;

    void update(unsigned dt) override;
    void draw() override;
};

class Second : public Dynamo::Scene {
    Dynamo::Sprite *text;

public:
    Second(Dynamo::Core modules) : Dynamo::Scene(modules) {};

    void on_entry() override;
    void on_exit() override;

    void update(unsigned dt) override;
    void draw() override;
};


// Definitions
void First::on_entry() {
    textures_->load_font("sentry", "assets/fonts/Sentry.ttf", 64);
    text = new Dynamo::Sprite(textures_->load_text(
        "Scene 1! Press Q to change scenes.", 
        "sentry",
        {255, 255, 255}
    ));
}

void First::on_exit() {
    delete text;
}

void First::update(unsigned dt) {
    if(inputs_->get_pressed_raw(Dynamo::INPUT_Q)) {
        set_scene<Second>();
    }
}

void First::draw() {
    display_->set_fill({255, 0, 255});
    display_->draw_sprite(nullptr, text, {500, 300});
}


void Second::on_entry() {
    textures_->load_font("sentry", "assets/fonts/Sentry.ttf", 64);
    text = new Dynamo::Sprite(textures_->load_text(
        "Scene 2! Press Q to change scenes.", 
        "sentry",
        {255, 255, 255}
    ));
}

void Second::on_exit() {
    delete text;
}

void Second::update(unsigned dt) {
    if(inputs_->get_pressed_raw(Dynamo::INPUT_Q)) {
        set_scene<First>();
    }
}

void Second::draw() {
    display_->set_fill({0, 100, 0});
    display_->draw_sprite(nullptr, text, {500, 300});
}

// Program entry point
int main(int argv, char **args) {
    Dynamo::Engine engine("Scene Test", false, 1000, 600);
    engine.push_scene<First>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}