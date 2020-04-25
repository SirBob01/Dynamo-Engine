#include "../src/Dynamo.h"
#include <vector>

class Game : public Dynamo::Scene {
    std::vector<Dynamo::Sprite *> commands;
    Dynamo::StreamID music_stream, ambient_stream;

    Dynamo::Sound *sfx;
    Dynamo::Sound *recorded;
    bool recording;

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
        inputs_->bind("start recording", Dynamo::INPUT_V);
        inputs_->bind("play recording", Dynamo::INPUT_B);

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
        recorded = new Dynamo::Sound(4194304);

        jukebox_->set_volume(0.5);
    };
    
    void on_exit() override {
        for(auto sprite : commands) {
            delete sprite;
        }
        jukebox_->clear();
        delete sfx;
        delete recorded;
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
        if(inputs_->get_pressed("start recording")) {
            // Reset record buffer
            for(int i = 0; i < recorded->length; i++) {
                recorded->samples[i] = 0;
            }
            recorded->write = 0;

            jukebox_->start_record();
            recording = true;
        }
        if(inputs_->get_released("start recording")) {
            jukebox_->start_record();
            recording = false;
        }
        if(inputs_->get_pressed("play recording")) {
            jukebox_->play_sound(recorded, 1.0);
        }

        if(recording) {
            jukebox_->stream_recorded(recorded);
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
    Dynamo::Engine engine("Audio Test", false, 640, 480);
    engine.push_scene<Game>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}