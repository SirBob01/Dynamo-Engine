#include "engine.h"

namespace Dynamo {
	Engine::Engine(int width, int height, std::string title) {
		SDL_Init(SDL_INIT_EVERYTHING);

		// Ugly ass singleton code
		modules = new Modules();
		modules->display = new Display(width, height, title);
		modules->textures = new Textures(modules->display->get_renderer());
		modules->jukebox = new Jukebox();
		modules->inputs = new Inputs();
		modules->clock = new Clock();

		running = false;
	}

	bool Engine::get_running() {
		return running;
	}

	Modules *Engine::get_modules() {
		return modules;
	}

	void Engine::push_scene(Scene *scene) {
		scene_stack.push(scene);
	}

	void Engine::run(int fps_cap) {
		modules->clock->tick();
		modules->inputs->poll();
		
		if(scene_stack.empty() || modules->inputs->get_quit()) {
			stop();
		}
		else {
			Scene *current_scene = scene_stack.top();
			current_scene->update();
			current_scene->draw();

			Scene *next_scene = current_scene->get_child();
			current_scene->set_child(nullptr);

			// Only pop a scene if it is "dead"
			// Allows layered scenes (e.g. A pause menu over gameplay)
			if(!current_scene->get_alive()) {
				delete current_scene;
				current_scene = nullptr;
				scene_stack.pop();

				if(next_scene != nullptr) {
					push_scene(next_scene);
				}
			}
		}
		
		// Play music
		modules->jukebox->stream_music();
		modules->jukebox->stream_ambient();

		modules->display->refresh();
		modules->clock->set_fps(fps_cap);
	}

	void Engine::start() {
		running = true;
	}

	void Engine::stop() {
		running = false;
	}

	void Engine::quit() {
		delete modules->display;
		delete modules->textures;
		delete modules->jukebox;
		delete modules->inputs;
		delete modules->clock;
		delete modules;

		SDL_Quit();
	}
}