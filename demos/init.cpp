#include <Dynamo.hpp>

int main() {
    Dynamo::Runtime runtime("Hello, world!", 640, 480);
    while (runtime.is_running()) {
        runtime.run();
    }
    return 0;
}
