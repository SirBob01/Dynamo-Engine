#include "scene.h"

namespace Dynamo {
    Scene::Scene() {
        next_ = nullptr;
        kill_ = true;
    }

    void Scene::load(Core &core) {
        return;
    }

    void Scene::unload(Core &core) {
        return;
    }

    void Scene::update(Core &core) {
        return;
    }
    
    void Scene::draw(Renderer &renderer) {
        return;
    }
}