#ifndef DYNAMO_SCENE_H_
#define DYNAMO_SCENE_H_

#include <SDL2/SDL.h>
#include <unordered_set>

#include "display.h"
#include "textures.h"
#include "jukebox.h"
#include "inputs.h"
#include "clock.h"

namespace Dynamo {
	// Holds references to singleton modules
	struct Modules {
		Display *display;
		Textures *textures;
		Jukebox *jukebox;
		Inputs *inputs;
		Clock *clock;
	};

	class Scene {	
		bool alive_;

	protected:
		Scene *parent_;
		Scene *child_;
		
		Display *display_;
		Textures *textures_;
		Jukebox *jukebox_;
		Inputs *inputs_;
		Clock *clock_;

	public:
		Scene(Scene *parent, Modules modules);
		virtual ~Scene();

		// Repackage modules to transition between scenes
		Modules get_modules();

		// Kill the current scene
		void kill();

		// Check if the current scene is alive
		bool get_alive();

		// Get the previus scene
		Scene *get_parent();
		
		// Get the next scene
		Scene *get_child();

		// Set the next scene after this one dies
		void set_child(Scene *next);

		// Handle scene logic
		virtual void update() = 0;

		// Draw all renderable scene objects
		virtual void draw() = 0;
	};
}

#endif