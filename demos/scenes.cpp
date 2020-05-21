#include "../src/Dynamo.h"

// Recommended: Forward declare all scenes to prevent circular dependency bugs
class First : public Dynamo::Scene {
    Dynamo::Sprite *text;

public:
    void load(Dynamo::Core &core) override;
    void unload(Dynamo::Core &core) override;

    void update(Dynamo::Core &core) override;
    void draw(Dynamo::Renderer &renderer) override;
};

class Second : public Dynamo::Scene {
    Dynamo::Sprite *text;

public:
    void load(Dynamo::Core &core) override;
    void unload(Dynamo::Core &core) override;

    void update(Dynamo::Core &core) override;
    void draw(Dynamo::Renderer &renderer) override;
};


// Definitions
void First::load(Dynamo::Core &core) {
    core.textures.load_font("sentry", "../assets/fonts/Sentry.ttf", 64);
    text = new Dynamo::Sprite(core.textures.load_text(
        "Scene 1! Press Q to load next scene.", 
        "sentry",
        {255, 255, 255}
    ));
}

void First::unload(Dynamo::Core &core) {
    delete text;
}

void First::update(Dynamo::Core &core) {
    if(core.inputs.get_pressed_raw(Dynamo::Input::Q)) {
        // Do not kill this scene, show it underneath
        // Demonstrates scene layering
        set_scene<Second>(false); 
    }
}

void First::draw(Dynamo::Renderer &renderer) {
    renderer.set_fill({255, 0, 255});
    renderer.draw_sprite(nullptr, text, {500, 300});
}


void Second::load(Dynamo::Core &core) {
    core.textures.load_font("sentry", "../assets/fonts/Sentry.ttf", 64);
    text = new Dynamo::Sprite(core.textures.load_text(
        "Scene 2! Press Q to unload.", 
        "sentry",
        {255, 255, 255}
    ));
}

void Second::unload(Dynamo::Core &core) {
    delete text;
}

void Second::update(Dynamo::Core &core) {
    if(core.inputs.get_pressed_raw(Dynamo::Input::Q)) {
        set_scene<First>();
    }
}

void Second::draw(Dynamo::Renderer &renderer) {
    renderer.set_fill({0, 100, 0, 100});
    renderer.draw_sprite(nullptr, text, {500, 200});
}

// Program entry point
int main(int argv, char **args) {
    Dynamo::Engine engine("Scene Test", 1000, 600);
    engine.push_scene<First>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}