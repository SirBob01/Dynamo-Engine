#include "../src/Dynamo.h"

class Game : public Dynamo::Scene {
    Dynamo::Sprite *input_display;
    Dynamo::Sprite *default_text;

public:
    void load(Dynamo::Core &core) override {
        input_display = nullptr;
        core.textures.load_font("sentry", "assets/fonts/Sentry.ttf", 64);
        default_text = new Dynamo::Sprite(core.textures.load_text(
            "Press anything to test input!", 
            "sentry",
            {255, 255, 255}
        ));
    };

    void update(Dynamo::Core &core) override {
        // Loop through all possible inputs and display its name if pressed
        core.inputs.each_code([&core, this](auto &code) {
            if(core.inputs.get_pressed_raw(code)) {
                if(input_display) {
                    delete input_display;
                }

                // Prevent memory leaks by deleting current renderable text
                core.textures.clear_textures();
                
                input_display = new Dynamo::Sprite(core.textures.load_text(
                    core.inputs.get_name(code), 
                    "sentry",
                    {255, 255, 255}
                ));
            }
        });
    };

    void draw(Dynamo::Renderer &renderer) override {
        renderer.set_fill({0, 0, 0});
        
        Dynamo::Sprite *target = input_display;
        if(!target) {
            target = default_text;
        }

        renderer.draw_sprite(
            nullptr, 
            target,
            {320, 240}
        );
    };
};

int main(int argv, char **args) {
    Dynamo::Engine engine("Input Test", 640, 480);
    engine.push_scene<Game>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}