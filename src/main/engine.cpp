#include "engine.h"

namespace Dynamo {
    Engine::Engine(int width, int height, std::string title) {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
            throw SDLError(SDL_GetError());
        }

        // Seed the random number generator
        Random::seed(std::time(nullptr));

        // Initialize singleton modules
        display_ = new Display(width, height, title);
        textures_ = new TextureManager(display_->get_renderer());
        jukebox_ = new Jukebox();
        inputs_ = new Inputs();
        clock_ = new Clock();

        scene_manager_ = new FSM();

        running_ = false;
    }

    Modules Engine::get_modules() {
        return {display_, textures_, jukebox_, inputs_, clock_};
    }

    bool Engine::get_running() {
        return running_;
    }

    void Engine::push_scene(Scene *scene) {
        scene_manager_->push_state(scene);
    }

    void Engine::run(int fps_cap) {
        clock_->tick();
        inputs_->poll();
        inputs_->scale_mouse_pos(
            display_->get_window_dimensions(), 
            display_->get_dimensions()
        );
        
        scene_manager_->update(clock_->get_delta());
        State *scene_state = scene_manager_->get_current();
        if(!scene_state || inputs_->get_quit()) {
            stop();
        }
        else {
            dynamic_cast<Scene *>(scene_state)->draw();
        }
        
        // Play soundtracks
        jukebox_->stream_music();
        jukebox_->stream_ambient();

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
        delete scene_manager_;
        delete display_;
        delete textures_;
        delete jukebox_;
        delete inputs_;
        delete clock_;
        
        SDL_Quit();
    }
}