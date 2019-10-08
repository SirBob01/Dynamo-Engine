#include "engine.h"

namespace Dynamo {
    Engine::Engine(int width, int height, std::string title) {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
            throw SDLError(SDL_GetError());
        }

        // Initialize singleton modules
        display_ = new Display(width, height, title);
        textures_ = new TextureManager(display_->get_renderer());
        jukebox_ = new Jukebox();
        inputs_ = new Inputs();
        clock_ = new Clock();

        running_ = false;
    }

    Modules Engine::get_modules() {
        return {display_, textures_, jukebox_, inputs_, clock_};
    }

    bool Engine::get_running() {
        return running_;
    }

    void Engine::push_scene(Scene *scene) {
        scene_stack_.push(scene);
    }

    void Engine::run(int fps_cap) {
        clock_->tick();
        inputs_->poll();
        
        if(scene_stack_.empty() || inputs_->get_quit()) {
            stop();
        }
        else {
            Scene *current_scene = scene_stack_.top();
            current_scene->update();
            current_scene->draw();

            Scene *next_scene = current_scene->get_child();
            current_scene->set_child(nullptr);

            // Only pop a scene if it is "dead" to
            // allow scene layering (e.g. pause menu over gameplay)
            if(!current_scene->get_alive()) {
                delete current_scene;
                current_scene = nullptr;
                scene_stack_.pop();

                if(next_scene != nullptr) {
                    push_scene(next_scene);
                }
            }
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
        delete display_;
        delete textures_;
        delete jukebox_;
        delete inputs_;
        delete clock_;
        
        SDL_Quit();
    }
}