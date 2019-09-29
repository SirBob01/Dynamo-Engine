#ifndef DYNAMO_COLOR_H_
#define DYNAMO_COLOR_H_

#include <SDL2/SDL.h>

namespace Dynamo {
	// Color format conversion functions
	uint32_t rgba_to_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	uint32_t rgb_to_hex(uint8_t r, uint8_t g, uint8_t b);

	SDL_Color hex_to_rgba(uint32_t rgba_color);
	SDL_Color hex_to_rgb(uint32_t rgb_color);
}


#endif