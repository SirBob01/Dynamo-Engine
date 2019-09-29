#include "clock.h"

namespace Dynamo {
	Clock::Clock() {
		frames = 0;
		previous = 0;
		current = 0;

		delta = 0.0f;
	}

	int Clock::get_frames() {
		return frames;
	}

	float Clock::get_delta() {
		return delta;
	}

	void Clock::tick() {
		// Call once only at the start of the game loop
		frames++;
		current = SDL_GetTicks();
		delta = current - previous;
	}

	void Clock::set_fps(int fps) {
		float cap_time = 1000.0/(float)fps;
		if(delta < cap_time) {
			SDL_Delay(cap_time - delta);
		}
		previous = current;
	}
}