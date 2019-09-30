#include "scene.h"

namespace Dynamo {
    Scene::Scene(Scene *parent, Modules modules) {
        // Unpack modules
        display_ = modules.display;
        textures_ = modules.textures;
        jukebox_ = modules.jukebox;
        inputs_ = modules.inputs;
        clock_ = modules.clock;

        parent_ = parent;
        child_ = nullptr;
        alive_ = true;
    }

    Scene::~Scene() {
        // Default black fill
        display_->set_fill({0, 0, 0});
        display_->set_borderfill({0, 0, 0});
    }

    Modules Scene::get_modules() {
        return {display_, textures_, jukebox_, inputs_, clock_};
    }

    void Scene::kill() {
        // Allows engine to transition to next scene
        alive_ = false;
    }

    bool Scene::get_alive() {
        return alive_;
    }

    Scene *Scene::get_parent() {
        return parent_;
    }

    Scene *Scene::get_child() {
        return child_;
    }

    void Scene::set_child(Scene *next) {
        child_ = next;
    }

    void Scene::update() {
        // Override
        // Update modules of this scene
    }

    void Scene::draw() {
        // Override
        // Draw scene sprites
    }
}