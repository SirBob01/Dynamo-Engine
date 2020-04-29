#include "scene.h"

namespace Dynamo {
    Scene::Scene(Core modules) : State() {
        // Unpack modules
        display_ = modules.display;
        renderer_ = modules.renderer;
        textures_ = modules.textures;
        jukebox_ = modules.jukebox;
        inputs_ = modules.inputs;
        clock_ = modules.clock;

        scene_registry_ = modules.scene_registry_;
        scenes_ = modules.scenes_;
    }

    Scene::~Scene() {
        // Default black fill
        renderer_->set_fill({0, 0, 0});
        renderer_->set_borderfill({0, 0, 0});
    }
}