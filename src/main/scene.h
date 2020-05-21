#ifndef DYNAMO_SCENE_H_
#define DYNAMO_SCENE_H_

#include <SDL2/SDL.h>
#include <vector>

#include "../core/display.h"
#include "../core/renderer.h"
#include "../core/textures.h"
#include "../core/jukebox.h"
#include "../core/inputs.h"
#include "../core/clock.h"
#include "../util/typeid.h"

namespace Dynamo {
    // Collection of references to core modules
    struct Core {
        Display &display;
        TextureManager &textures;
        Jukebox &jukebox;
        InputHandler &inputs;
        Clock &clock;
    };

    // Base class for game scenes (menus, cutscenes, gameplay)
    class Scene {
        TypeID *registry_;
        std::vector<Scene *> *scenes_;

        Scene *next_; // Pointer to the next scene
        bool kill_;   // Should kill on transition?
        bool draw_;   // Keep drawing if still alive?

        friend class Engine;

    public:
        Scene();
        virtual ~Scene() = default;

        // Set the next scene
        template <class S>
        void set_scene(bool kill=true, bool drawing=false) {
            unsigned type_id = registry_->get_id<S>();
            if(type_id >= scenes_->size()) {
                S *scene = new S();
                scene->registry_ = registry_;
                scene->scenes_ = scenes_;
                scenes_->push_back(scene);
            }
            next_ = (*scenes_)[type_id];
            next_->draw_ = true;

            kill_ = kill;
            draw_ = drawing;
        };

        // Initialize the scene and the necessary objects
        virtual void load(Core &core);

        // Destroy all objects and free allocated memory
        virtual void unload(Core &core);

        // Update scene state at every frame
        virtual void update(Core &core);

        // Draw all renderable objects
        virtual void draw(Renderer &renderer);
    };
}

#endif