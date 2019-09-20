#include "color.h"

uint32_t rgba_to_hex(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	return r << 24 | g << 16 | b << 8 | a;
}

uint32_t rgb_to_hex(uint8_t r, uint8_t g, uint8_t b) {
	return r << 16 | g << 8 | b;
}

SDL_Color hex_to_rgba(uint32_t rgba_color) {
	return {
		(uint8_t) ((rgba_color & 0xFF000000) >> 24), 
		(uint8_t) ((rgba_color & 0x00FF0000) >> 16),
		(uint8_t) ((rgba_color & 0x0000FF00) >> 8),
		(uint8_t) (rgba_color & 0x000000FF)
	};
}

SDL_Color hex_to_rgb(uint32_t rgb_color) {
	return {
		(uint8_t) ((rgb_color & 0x00FF0000) >> 16), 
		(uint8_t) ((rgb_color & 0x0000FF00) >> 8),
		(uint8_t) (rgb_color & 0x000000FF)
	};
}