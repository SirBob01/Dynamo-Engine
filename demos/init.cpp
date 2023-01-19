#include <Dynamo.hpp>

int main() {
    Dynamo::ApplicationConfiguration config;
    config.title = "Hello, world!";
    config.width = 640;
    config.height = 480;
    config.asset_directory = "../assets/";

    Dynamo::Application app(config);
    while (app.is_running()) {
        app.run();
    }
    return 0;
}
