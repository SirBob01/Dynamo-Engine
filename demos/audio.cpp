#include "../src/Dynamo.h"
#include <vector>

class Game : public Dynamo::Scene {
    std::vector<Dynamo::Sprite *> commands;
    int music_stream;
    int ambient_stream;

    Dynamo::Sound *sfx;

public:
    Game(Dynamo::Core modules) : Dynamo::Scene(modules) {};
    
    void on_entry() override {
        textures_->load_font("sentry", "assets/fonts/Sentry.ttf", 32);

        inputs_->bind("play music", Dynamo::INPUT_Q);
        inputs_->bind("play rain", Dynamo::INPUT_W);
        inputs_->bind("play boom sfx", Dynamo::INPUT_E);
        inputs_->bind("skip music", Dynamo::INPUT_A);
        inputs_->bind("fadeout music", Dynamo::INPUT_S);
        inputs_->bind("toggle pause", Dynamo::INPUT_P);
        inputs_->bind("clear music stream", Dynamo::INPUT_C);

        std::vector<std::string> all_binds = inputs_->get_bind_keys();

        for(auto bind : all_binds) {
            Dynamo::INPUT_CODE input_name = inputs_->get_bind(bind);
            commands.push_back(
                new Dynamo::Sprite(
                    textures_->load_text(
                        "Press " + inputs_->get_name(input_name) + " to " + bind,
                        "sentry",
                        {0, 0, 0}
                    )
                )
            );
        }

        ambient_stream = jukebox_->generate_stream();
        music_stream = jukebox_->generate_stream();
        sfx = jukebox_->load_sound("assets/audio/fusion_boom.ogg");

        jukebox_->set_volume(0.5);
    };
    
    void on_exit() override {
        for(auto sprite : commands) {
            delete sprite;
        }
        jukebox_->clear();
        jukebox_->destroy_sound(sfx);
    };

    void update(unsigned dt) override {
        if(inputs_->get_pressed("play music")) {
            jukebox_->queue_stream(
                "assets/audio/test_mixer.ogg", 
                music_stream, 
                5, 0, -1 // Fade-in 5 seconds, abrupt stop, loop forever
            );
        }
        if(inputs_->get_pressed("skip music")) {
            jukebox_->skip_stream(music_stream, 0);
        }
        if(inputs_->get_pressed("fadeout music")) {
            jukebox_->skip_stream(music_stream, 5);
        }
        if(inputs_->get_pressed("clear music stream")) {
            jukebox_->clear_stream(music_stream);
        }
        if(inputs_->get_pressed("play rain")) {
            jukebox_->queue_stream("assets/audio/rain.ogg", ambient_stream);
        }
        if(inputs_->get_pressed("play boom sfx")) {
            jukebox_->play_sound(sfx, 0.5);
        }
        if(inputs_->get_pressed("toggle pause")) {
            if(jukebox_->is_playing()) {
                jukebox_->pause();
            }
            else {
                jukebox_->play();
            }
        }
    };

    void draw() override {
        display_->set_fill({255, 0, 255});
        float index = 0.0f;
        for(auto text : commands) {
            display_->draw_sprite(
                nullptr, 
                text,
                {320.0f, 50.0f + (index * 32.0f)}
            );
            index++;
        }
    };
};

int main(int argv, char **args) {
    Dynamo::Engine game_engine("Audio Test", false, 640, 480);
    game_engine.push_scene<Game>();
    game_engine.start();

    while(game_engine.is_running()) {
        game_engine.run();
    }

    game_engine.quit();
    return 0;
}