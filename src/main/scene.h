#ifndef DYNAMO_SCENE_H_
#define DYNAMO_SCENE_H_

#include <SDL2/SDL.h>
#include <unordered_set>

#include "../state/fsm.h"
#include "../state/state.h"
#include "../core/display.h"
#include "../core/textures.h"
#include "../core/jukebox.h"
#include "../core/inputs.h"
#include "../core/clock.h"
#include "../util/typeid.h"

namespace Dynamo {
    // Holds references to core data passed between scenes
    struct Core {
        Display *display;
        TextureManager *textures;
        Jukebox *jukebox;
        Inputs *inputs;
        Clock *clock;

        TypeID *scene_registry_;
        std::vector<State *> *scenes_;
    };

    // Base class for game scenes (menus, cutscenes, gameplay)
    class Scene : public State {
        TypeID *scene_registry_;
        std::vector<State *> *scenes_;

    protected:
        Display *display_;
        TextureManager *textures_;
        Jukebox *jukebox_;
        Inputs *inputs_;
        Clock *clock_;

    public:
        Scene(Core modules);
        virtual ~Scene();

        // Set the next scene
        template <class S>
        void set_scene() {
            unsigned type_id = scene_registry_->get_id<S>();
            if(type_id >= scenes_->size()) {
                scenes_->push_back(
                    new S({
                        display_, 
                        textures_, 
                        jukebox_, 
                        inputs_, 
                        clock_,

                        scene_registry_,
                        scenes_
                    })
                );
            }
            set_next((*scenes_)[type_id]);
        };

        // Draw all renderable scene objects
        virtual void draw() = 0;
    };
}

#endif