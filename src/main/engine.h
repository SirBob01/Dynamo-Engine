#ifndef DYNAMO_ENGINE_H_
#define DYNAMO_ENGINE_H_

#include <SDL2/SDL.h>

#include <vector>
#include <string>
#include <ctime>

#include "scene.h"
#include "../util/random.h"

namespace Dynamo {
    enum class EngineFlag {
        None       = 0,
        FullScreen = 1 << 0, 
        VSync      = 1 << 1,
    };

    inline unsigned operator&(EngineFlag lhs, EngineFlag rhs) {
        using T = unsigned;
        return static_cast<T>(lhs) & static_cast<T>(rhs);
    }

    inline EngineFlag operator|(EngineFlag lhs, EngineFlag rhs) {
        using T = unsigned;
        return static_cast<EngineFlag>(
            static_cast<T>(lhs) | static_cast<T>(rhs)
        );
    }

    inline EngineFlag operator|=(EngineFlag &lhs, EngineFlag rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    class Engine {
        Display *display_;
        Renderer *renderer_;
        TextureManager *textures_;
        Jukebox *jukebox_;
        InputHandler *inputs_;
        Clock *clock_;

        TypeID registry_;
        std::vector<Scene *> scenes_;
        std::vector<Scene *> active_;

        bool running_;

    public:
        Engine(std::string title, int width=0, int height=0, 
               EngineFlag flags=EngineFlag::None);
        ~Engine();

        // Check if the engine is running
        bool is_running();

        // Add a new scene to the stack
        template <class S>
        void push_scene() {
            unsigned type_id = registry_.get_id<S>();
            if(type_id >= scenes_.size()) {
                S *scene = new S();
                scene->registry_ = &registry_;
                scene->scenes_ = &scenes_;
                scenes_.push_back(scene);
            }
            Core core = {
                *display_,
                *textures_,
                *jukebox_,
                *inputs_,
                *clock_
            };
            scenes_[type_id]->load(core);
            active_.push_back(scenes_[type_id]);
        };
        
        // Perform a single frame update
        void run(int fps_cap=0);

        // Start the application
        void start();

        // Pause the application
        void stop();
    };
}

#endif