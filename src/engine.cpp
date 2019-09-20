#include "engine.h"

GameEngine::GameEngine(int width, int height, std::string title) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// Should probably find a better version to do this...
	modules = new GameModules();
	modules->display = new GameDisplay(width, height, title);
	modules->textures = new GameTextures(modules->display->get_renderer());
	modules->jukebox = new GameJukebox();
	modules->inputs = new GameInputs();
	modules->clock = new GameClock();

	running = false;
}

bool GameEngine::get_running() {
	return running;
}

GameModules *GameEngine::get_modules() {
	return modules;
}

void GameEngine::push_scene(GameScene *scene) {
	scene_stack.push(scene);
}

void GameEngine::run(int fps_cap) {
	modules->clock->tick();
	modules->inputs->poll();
	
	if(scene_stack.empty() || modules->inputs->get_quit()) {
		stop();
	}
	else {
		GameScene *current_scene = scene_stack.top();
		current_scene->update();
		current_scene->draw();

		GameScene *next_scene = current_scene->get_child();
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

void GameEngine::start() {
	running = true;
}

void GameEngine::stop() {
	running = false;
}

void GameEngine::quit() {
	delete modules->display;
	delete modules->textures;
	delete modules->jukebox;
	delete modules->inputs;
	delete modules->clock;
	delete modules;

	SDL_Quit();
}