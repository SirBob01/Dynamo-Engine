#ifndef DYNAMO_ENGINE_H_
#define DYNAMO_ENGINE_H_

#include <SDL2/SDL.h>

#include <string>
#include <stack>

#include "scene.h"

namespace Dynamo {
	class Engine {
		Display *display_;
		Textures *textures_;
		Jukebox *jukebox_;
		Inputs *inputs_;
		Clock *clock_;

		std::stack<Scene *> scene_stack_;

		bool running_;

	public:
		Engine(int width, int height, std::string title);

		// Check if the engine is running
		bool get_running();

		// Get the module components for each scene
		Modules get_modules();

		// Add a new scene to the stack
		void push_scene(Scene *scene);
		
		// Perform a single frame update
		void run(int fps_cap);

		// Start the application
		void start();

		// Pause the application
		void stop();

		// Uninitialize all modules and free memory
		void quit();
	};
}

#endif