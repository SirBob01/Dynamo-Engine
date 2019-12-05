#include "scene.h"

namespace Dynamo {
    Scene::Scene(Modules modules) : State() {
        // Unpack modules
        display_ = modules.display;
        textures_ = modules.textures;
        jukebox_ = modules.jukebox;
        inputs_ = modules.inputs;
        clock_ = modules.clock;
    }

    Scene::~Scene() {
        // Default black fill
        display_->set_fill({0, 0, 0});
        display_->set_borderfill({0, 0, 0});
    }

    Modules Scene::get_modules() {
        return {display_, textures_, jukebox_, inputs_, clock_};
    }
}