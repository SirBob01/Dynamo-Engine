#include "../src/Dynamo.h"
#include <vector>

class Game : public Dynamo::Scene {
    std::vector<Dynamo::Sprite *> commands;

    Dynamo::SoundStream music, ambient;
    Dynamo::Sound sfx, recorded;

public:
    void load(Dynamo::Core &core) override {
        core.textures.load_font("sentry", "../assets/fonts/Sentry.ttf", 32);

        core.inputs.bind("play music", Dynamo::Input::Q);
        core.inputs.bind("play rain", Dynamo::Input::W);
        core.inputs.bind("play boom sfx", Dynamo::Input::E);
        core.inputs.bind("skip music", Dynamo::Input::A);
        core.inputs.bind("fadeout music", Dynamo::Input::S);
        core.inputs.bind("toggle pause", Dynamo::Input::P);
        core.inputs.bind("clear music stream", Dynamo::Input::C);
        core.inputs.bind("start recording (hold)", Dynamo::Input::V);
        core.inputs.bind("play recording", Dynamo::Input::B);

        core.inputs.each_bind([&core, this](auto &bind, auto &code) {
            commands.push_back(
                new Dynamo::Sprite(
                    core.textures.load_text(
                        "Press " + core.inputs.get_name(code) + " to " + bind,
                        "sentry",
                        {0, 0, 0}
                    )
                )
            );
        });

        ambient = core.jukebox.generate_stream();
        music = core.jukebox.generate_stream();
        
        sfx = core.jukebox.load_sound("../assets/audio/fusion_boom.ogg");
        
        core.jukebox.set_volume(0.5);
    };
    
    void unload(Dynamo::Core &core) override {
        for(auto sprite : commands) {
            delete sprite;
        }
        core.jukebox.clear();
    };

    void update(Dynamo::Core &core) override {
        // Test stereo panning
        // Listener is at the center of the screen
        Dynamo::Vec2D position = core.inputs.get_mouse_pos() - core.display.get_dimensions()/2.0;
        core.jukebox.set_stream_position(music, position);
        
        if(core.inputs.get_pressed("play music")) {
            core.jukebox.queue_stream(
                "../assets/audio/test_mixer.ogg", 
                music, 
                5, 5, -1 // Fade-in 5 seconds, abrupt stop
            );
        }
        if(core.inputs.get_pressed("skip music")) {
            core.jukebox.skip_stream(music, 3);
        }
        if(core.inputs.get_pressed("fadeout music")) {
            core.jukebox.skip_stream(music, 5);
        }
        if(core.inputs.get_pressed("clear music stream")) {
            core.jukebox.clear_stream(music);
        }
        if(core.inputs.get_pressed("play rain")) {
            core.jukebox.queue_stream("../assets/audio/rain.ogg", ambient);
        }
        if(core.inputs.get_pressed("play boom sfx")) {
            core.jukebox.play_sound(sfx, 0.5, position);
        }
        if(core.inputs.get_pressed("toggle pause")) {
            if(core.jukebox.is_stream_playing(music)) {
                core.jukebox.pause_stream(music);
            }
            else {
                core.jukebox.play_stream(music);
            }
        }
        if(core.inputs.get_pressed("start recording (hold)")) {
            // Reset record buffer
            recorded.clear();
            core.jukebox.start_record();
        }
        if(core.inputs.get_released("start recording (hold)")) {
            core.jukebox.pause_record();
        }
        if(core.inputs.get_pressed("play recording")) {
            core.jukebox.play_sound(recorded, 1.0);
        }

        if(core.jukebox.is_recording()) {
            core.jukebox.stream_recorded(recorded);
        }
    };

    void draw(Dynamo::Renderer &renderer) override {
        renderer.set_fill({255, 0, 255});
        float index = 0.0f;
        for(auto text : commands) {
            renderer.draw_sprite(
                nullptr, 
                text,
                {320.0f, 50.0f + (index * 32.0f)}
            );
            index++;
        }
    };
};

int main(int argv, char **args) {
    Dynamo::Engine engine("Audio Test", 640, 480);
    engine.push_scene<Game>();

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}