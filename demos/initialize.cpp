#include "../src/Dynamo.h"

// Scene declaration
class MyScene : public Dynamo::Scene {
public:
    void load(Dynamo::Core &core) override;
    
    void unload(Dynamo::Core &core) override;
    
    void update(Dynamo::Core &core) override;

    void draw(Dynamo::Renderer &renderer) override;
};

// Scene method overrides
void MyScene::load(Dynamo::Core &core) {}

void MyScene::unload(Dynamo::Core &core) {}

void MyScene::update(Dynamo::Core &core) {}

void MyScene::draw(Dynamo::Renderer &renderer) {
    Dynamo::Color white = {255, 255, 255};
    renderer.set_fill(white);
}

// Entry point function
int main(int argc, char **argv) {
    // Initial window size and logical dimensions
    int width = 640, height = 480;

    Dynamo::Engine engine("Hello, world!", width, height);
    engine.push_scene<MyScene>(); // Load the initial scene

    // Game loop
    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}