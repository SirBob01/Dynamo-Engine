#include <Dynamo.hpp>

int main() {
    Dynamo::ApplicationSettings settings;
    settings.title = "Hello, world!";
    settings.window_width = 640;
    settings.window_height = 480;
    settings.root_asset_directory = "../assets/";

    Dynamo::Application app(settings);
    while (app.is_running()) {
        app.update();
    }
    return 0;
}
