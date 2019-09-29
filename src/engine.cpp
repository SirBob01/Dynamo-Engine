#include "engine.h"

namespace Dynamo {
	Engine::Engine(int width, int height, std::string title) {
		SDL_Init(SDL_INIT_EVERYTHING);

		// Ugly ass singleton code
		modules_ = new Modules();
		modules_->display = new Display(width, height, title);
		modules_->textures = new Textures(modules_->display->get_renderer());
		modules_->jukebox = new Jukebox();
		modules_->inputs = new Inputs();
		modules_->clock = new Clock();

		running_ = false;
	}

	bool Engine::get_running() {
		return running_;
	}

	Modules *Engine::get_modules() {
		return modules_;
	}

	void Engine::push_scene(Scene *scene) {
		scene_stack_.push(scene);
	}

	void Engine::run(int fps_cap) {
		modules_->clock->tick();
		modules_->inputs->poll();
		
		if(scene_stack_.empty() || modules_->inputs->get_quit()) {
			stop();
		}
		else {
			Scene *current_scene = scene_stack_.top();
			current_scene->update();
			current_scene->draw();

			Scene *next_scene = current_scene->get_child();
			current_scene->set_child(nullptr);

			// Only pop a scene if it is "dead"
			// Allows layered scenes (e.g. A pause menu over gameplay)
			if(!current_scene->get_alive()) {
				delete current_scene;
				current_scene = nullptr;
				scene_stack_.pop();

				if(next_scene != nullptr) {
					push_scene(next_scene);
				}
			}
		}
		
		// Play music
		modules_->jukebox->stream_music();
		modules_->jukebox->stream_ambient();

		modules_->display->refresh();
		modules_->clock->set_fps(fps_cap);
	}

	void Engine::start() {
		running_ = true;
	}

	void Engine::stop() {
		running_ = false;
	}

	void Engine::quit() {
		delete modules_->display;
		delete modules_->textures;
		delete modules_->jukebox;
		delete modules_->inputs;
		delete modules_->clock;
		delete modules_;

		SDL_Quit();
	}
}