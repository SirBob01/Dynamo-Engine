#ifndef GAME_CLOCK
#define GAME_CLOCK

#include <SDL2/SDL.h>

class GameClock {
	int frames;
	int previous;
	int current;

	float delta;

public:
	GameClock();

	int get_frames();
	float get_delta();

	void tick();
	void set_fps(int fps);
};

#endif