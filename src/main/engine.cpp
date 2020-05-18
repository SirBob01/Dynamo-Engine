#include "engine.h"

namespace Dynamo {
    Engine::Engine(std::string title, bool fullscreen, bool vsync,
                   int width, int height) {
        if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
            throw SDLError();
        }

        // Seed the random number generator
        Random::seed(std::time(nullptr));

        // Initialize singleton modules
        clock_ = new Clock();
        display_ = new Display(width, height, title, fullscreen, vsync);
        renderer_ = new Renderer(display_);
        textures_ = new TextureManager(display_->get_renderer());
        jukebox_ = new Jukebox();
        inputs_ = new Inputs();

        running_ = true;
    }

    Engine::~Engine() {
        // Clean up
        for(auto &scene : scenes_) {
            delete scene;
        }
        delete display_;
        delete renderer_;
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
        Core core = {
            *display_,
            *textures_,
            *jukebox_,
            *inputs_,
            *clock_
        };

        if(!active_.empty()) {
            Scene *current = active_.back();
            current->update(core);

            for(auto &scene : active_) {
                scene->draw(*renderer_);
            }

            if(current->next_) {
                Scene *next = current->next_;
                current->next_ = nullptr;

                if(current->kill_) {
                    current->unload(core);
                    active_.pop_back();
                }
                if(next->kill_) {
                    next->load(core);
                }

                int search = -1;
                for(int i = 0; i < active_.size(); i++) {
                    if(active_[i] == next) {
                        search = i;
                    }
                }
                if(search == -1) {
                    active_.push_back(next);
                }
                else {
                    std::swap(active_.back(), active_[search]);
                }
            }
        }

        jukebox_->update(clock_->get_delta());
        renderer_->refresh();
        clock_->set_fps(fps_cap);
        
        if(inputs_->get_quit() || active_.empty()) {
            for(auto &scene : active_) {
                scene->unload(core);
            }
            stop();
        }
    }

    void Engine::start() {
        running_ = true;
    }

    void Engine::stop() {
        running_ = false;
    }
}