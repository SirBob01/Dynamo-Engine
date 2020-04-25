#include "../src/Dynamo.h"

class Game : public Dynamo::Scene {
    Dynamo::Sprite *input_display;
    Dynamo::Sprite *default_text;

public:
    Game(Dynamo::Core modules) : Dynamo::Scene(modules) {};
    
    void on_entry() override {
        input_display = nullptr;
        textures_->load_font("sentry", "assets/fonts/Sentry.ttf", 64);
        default_text = new Dynamo::Sprite(textures_->load_text(
            "Press anything to test input!", 
            "sentry",
            {255, 255, 255}
        ));
    };

    void update(unsigned dt) override {
        // Loop through all possible inputs and display its name if pressed
        for(int i = 0; i < Dynamo::INPUT_LEN + 1; i++) {
            Dynamo::INPUT_CODE code = static_cast<Dynamo::INPUT_CODE>(i);
            if(inputs_->get_pressed_raw(code)) {
                if(input_display) {
                    delete input_display;
                }

                // Prevent memory leaks by deleting current renderable text
                textures_->clear_textures();
                
                input_display = new Dynamo::Sprite(textures_->load_text(
                    inputs_->get_name(code), 
                    "sentry",
                    {255, 255, 255}
                ));
            }
        }
    };

    void draw() override {
        display_->set_fill({0, 0, 0});
        
        Dynamo::Sprite *target = input_display;
        if(!target) {
            target = default_text;
        }

        display_->draw_sprite(
            nullptr, 
            target,
            {320, 240}
        );
    };
};

int main(int argv, char **args) {
    Dynamo::Engine engine("Input Test", false, 640, 480);
    engine.push_scene<Game>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}