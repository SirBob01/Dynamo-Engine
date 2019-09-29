#ifndef DYNAMO_COLOR_H_
#define DYNAMO_COLOR_H_

#include <SDL2/SDL.h>

namespace Dynamo {
	struct Color {
		uint8_t r, g, b; 
		uint8_t a = 255; // Default alpha value

		// Convert color to hexidecimal
		uint32_t get_hex();
	};

	Color color_from_hex(uint32_t rgba);
}


#endif