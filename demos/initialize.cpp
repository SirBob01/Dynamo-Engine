#include "../src/Dynamo.h"

class MyScene : public Dynamo::Scene {
public:
    MyScene(Dynamo::Core modules) : Dynamo::Scene(modules) {};
    
    void on_entry() override;
    
    void on_exit() override;
    
    void update(unsigned dt) override;

    void draw() override;
};

void MyScene::on_entry() {}

void MyScene::on_exit() {}

void MyScene::update(unsigned dt) {}

void MyScene::draw() {
    display_->set_fill({255, 255, 255});
}

int main(int argc, char **argv) {
    int width = 640, height = 480;
    bool fullscreen = false;

    Dynamo::Engine engine("Hello, world!", fullscreen, width, height);
    engine.push_scene<MyScene>(); // Initial scene

    while(engine.is_running()) {
        engine.run();
    }
    return 0;
}