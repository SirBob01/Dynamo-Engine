#ifndef GAME_SCENE
#define GAME_SCENE

#include <SDL2/SDL.h>
#include <unordered_set>

#include "display.h"
#include "textures.h"
#include "jukebox.h"
#include "inputs.h"
#include "clock.h"

// Objects that need to be passed from scene to scene
struct GameModules {
	GameDisplay *display;
	GameTextures *textures;
	GameJukebox *jukebox;
	GameInputs *inputs;
	GameClock *clock;
};

class GameScene {	
	bool alive;

protected:
	GameScene *parent_scene;
	GameScene *child_scene;
	
	GameModules *modules;

public:
	GameScene(GameScene *p, GameModules *c);
	virtual ~GameScene();

	void kill();
	bool get_alive();

	GameScene *get_parent();
	GameScene *get_child();

	void set_child(GameScene *next);

	virtual void update();
	virtual void draw();
};

#endif