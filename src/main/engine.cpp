#include "engine.h"

namespace Dynamo {
    Engine::Engine(std::string title, bool fullscreen, 
                   int width, int height) {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
            throw SDLError(SDL_GetError());
        }

        // Seed the random number generator
        Random::seed(std::time(nullptr));

        // Initialize singleton modules
        display_ = new Display(width, height, title, fullscreen);
        textures_ = new TextureManager(display_->get_renderer());
        jukebox_ = new Jukebox();
        inputs_ = new Inputs();
        clock_ = new Clock();

        scene_manager_ = new FSM();

        running_ = false;
    }

    bool Engine::is_running() {
        return running_;
    }

    void Engine::run(int fps_cap) {
        clock_->tick();
        inputs_->poll();
        
        scene_manager_->update(clock_->get_delta());
        State *scene_state = scene_manager_->get_current();
        if(!scene_state || inputs_->get_quit()) {
            stop();
        }
        else {
            dynamic_cast<Scene *>(scene_state)->draw();
        }
        
        jukebox_->update();
        display_->refresh();
        clock_->set_fps(fps_cap);
    }

    void Engine::start() {
        running_ = true;
    }

    void Engine::stop() {
        running_ = false;
    }

    void Engine::quit() {
        // Clean up
        for(auto &scene : scenes_) {
            delete scene;
        }
        delete scene_manager_;
        delete display_;
        delete textures_;
        delete jukebox_;
        delete inputs_;
        delete clock_;
        
        SDL_Quit();
    }
}