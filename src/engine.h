#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <SDL2/SDL.h>
#include <string>
#include <stack>

#include "scene.h"

class GameEngine {
	GameModules *modules;
	std::stack<GameScene *> scene_stack;

	bool running;

public:
	GameEngine(int width, int height, std::string title);

	bool get_running();
	GameModules *get_modules();

	void push_scene(GameScene *scene);
	
	void run(int fps_cap);
	void start();
	void stop();
	void quit();
};

#endif