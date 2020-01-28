#ifndef DYNAMO_ENGINE_H_
#define DYNAMO_ENGINE_H_

#include <SDL2/SDL.h>

#include <vector>
#include <string>
#include <ctime>

#include "scene.h"
#include "../state/fsm.h"
#include "../util/random.h"

namespace Dynamo {
    class Engine {
        Display *display_;
        TextureManager *textures_;
        Jukebox *jukebox_;
        Inputs *inputs_;
        Clock *clock_;

        FSM *scene_manager_;
        TypeID scene_registry_;
        std::vector<State *> scenes_;

        bool running_;

    public:
        Engine(std::string title, bool fullscreen, 
               int width=0, int height=0);

        // Check if the engine is running
        bool get_running();

        // Add a new scene to the stack
        template <class S, typename ... Arg>
        void push_scene(Arg ... args) {
            unsigned type_id = scene_registry_.get_id<S>();
            if(type_id >= scenes_.size()) {
                scenes_.push_back(
                    new S(
                        {
                            display_, 
                            textures_, 
                            jukebox_, 
                            inputs_, 
                            clock_,

                            &scene_registry_,
                            &scenes_
                        }, 
                        args ...
                    )
                );
            }
            scene_manager_->push_state(scenes_[type_id]);
        }
        
        // Perform a single frame update
        void run(int fps_cap=0);

        // Start the application
        void start();

        // Pause the application
        void stop();

        // Uninitialize all modules and free memory
        void quit();
    };
}

#endif