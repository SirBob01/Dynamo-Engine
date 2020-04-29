#include "engine.h"

namespace Dynamo {
    Engine::Engine(std::string title, bool fullscreen, 
                   int width, int height) {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
            throw SDLError();
        }

        // Seed the random number generator
        Random::seed(std::time(nullptr));

        // Initialize singleton modules
        clock_ = new Clock();
        display_ = new Display(width, height, title, fullscreen);
        renderer_ = new Renderer(display_->get_window());
        textures_ = new TextureManager(renderer_->get_renderer());
        jukebox_ = new Jukebox(clock_);
        inputs_ = new Inputs();

        scene_manager_ = new FSM();

        running_ = true;
    }

    Engine::~Engine() {
        // Clean up
        for(auto &scene : scenes_) {
            delete scene;
        }
        delete scene_manager_;
        delete renderer_;
        delete display_;
        delete textures_;
        delete jukebox_;
        delete inputs_;
        delete clock_;
        
        SDL_Quit();
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
        renderer_->refresh();
        clock_->set_fps(fps_cap);
    }

    void Engine::start() {
        running_ = true;
    }

    void Engine::stop() {
        running_ = false;
    }
}