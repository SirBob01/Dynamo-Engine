#ifndef DYNAMO_ENGINE_H_
#define DYNAMO_ENGINE_H_

#include <SDL2/SDL.h>

#include <string>

#include "scene.h"
#include "../state/fsm.h"

namespace Dynamo {
    class Engine {
        Display *display_;
        TextureManager *textures_;
        Jukebox *jukebox_;
        Inputs *inputs_;
        Clock *clock_;

        FSM *scene_manager_;

        bool running_;

    public:
        Engine(int width, int height, std::string title);

        // Get the module components for each scene
        Modules get_modules();

        // Check if the engine is running
        bool get_running();

        // Add a new scene to the stack
        void push_scene(Scene *scene);
        
        // Perform a single frame update
        void run(int fps_cap);

        // Start the application
        void start();

        // Pause the application
        void stop();

        // Uninitialize all modules and free memory
        void quit();
    };
}

#endif