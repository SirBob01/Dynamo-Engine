#include <Dynamo.hpp>

int main() {
    Dynamo::Engine engine("Hello, world!", 640, 480);
    while (engine.is_running()) {
        engine.run();
    }
    return 0;
}
