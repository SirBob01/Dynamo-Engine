#include <Dynamo.hpp>

int main() {
    Dynamo::ApplicationConfiguration config;
    config.title = "Jukebox!";
    config.width = 640;
    config.height = 480;
    config.asset_directory = "../assets/";

    Dynamo::Application app(config);
    Dynamo::Sound::Jukebox &jukebox = app.get_jukebox();
    Dynamo::Asset<Dynamo::Sound::Sound> sound =
        app.get_assets().get<Dynamo::Sound::Sound>("./audio/test_mixer.ogg");

    Dynamo::Sound::DynamicMaterial dynamic_material;
    dynamic_material.volume = 1.0;
    dynamic_material.position.x = 0;
    dynamic_material.position.y = 0;
    dynamic_material.position.z = 0;
    dynamic_material.filters.push_back(
        std::make_unique<Dynamo::Sound::Binaural>(jukebox.get_hrtf()));
    dynamic_material.filters.push_back(
        std::make_unique<Dynamo::Sound::Attenuation>(0, 450));

    Dynamo::Sound::StaticMaterial static_material;
    static_material.volume = 1.0;

    Dynamo::Sound::Listener listener = jukebox.get_listeners().create();
    while (app.is_running()) {
        const Dynamo::Core &core = app.get_core();
        if (core.input.is_pressed(Dynamo::KeyCode::Space)) {
            jukebox.play(*sound, static_material);
        }
        Dynamo::Vec2 center = core.display.get_size() / 2;
        auto &props = jukebox.get_listeners().get_properties(listener);
        props.position.x = center.x;
        props.position.z = center.y;

        Dynamo::Vec2 mouse_position = core.input.get_mouse_position();
        dynamic_material.position.x = mouse_position.x;
        dynamic_material.position.z = mouse_position.y;
        app.run();
    }
    return 0;
}
