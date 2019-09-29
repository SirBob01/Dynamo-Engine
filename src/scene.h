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
	// Singleton objects that need to be passed from scene to scene
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
		
		Modules *modules_;

	public:
		Scene(Scene *parent, Modules *modules);
		virtual ~Scene();

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
		virtual void update();

		// Draw all renderable scene objects
		virtual void draw();
	};
}

#endif