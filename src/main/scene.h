#ifndef DYNAMO_SCENE_H_
#define DYNAMO_SCENE_H_

#include <SDL2/SDL.h>
#include <unordered_set>

#include "../state/state.h"
#include "../core/display.h"
#include "../core/textures.h"
#include "../core/jukebox.h"
#include "../core/inputs.h"
#include "../core/clock.h"

namespace Dynamo {
    // Holds references to singleton modules
    struct Modules {
        Display *display;
        TextureManager *textures;
        Jukebox *jukebox;
        Inputs *inputs;
        Clock *clock;
    };

    class Scene : public State {
    protected:
        Display *display_;
        TextureManager *textures_;
        Jukebox *jukebox_;
        Inputs *inputs_;
        Clock *clock_;

    public:
        Scene(Modules modules);
        virtual ~Scene();

        // Repackage modules to transition between scenes
        Modules get_modules();

        // Draw all renderable scene objects
        virtual void draw() = 0;
    };
}

#endif