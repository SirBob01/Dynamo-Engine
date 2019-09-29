#ifndef DYNAMO_CLOCK_H_
#define DYNAMO_CLOCK_H_

#include <SDL2/SDL.h>

namespace Dynamo {
	class Clock {
		int frames;
		int previous;
		int current;

		float delta;

	public:
		Clock();

		int get_frames();
		float get_delta();

		void tick();
		void set_fps(int fps);
	};
}

#endif