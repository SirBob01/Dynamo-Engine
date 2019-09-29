#include "color.h"

namespace Dynamo {
	uint32_t Color::get_hex() {
		return r << 24 | g << 16 | b << 8 | a;
	}

	Color color_from_hex(uint32_t rgba) {
		return {
			(uint8_t)((rgba & 0xFF000000) >> 24), 
			(uint8_t)((rgba & 0x00FF0000) >> 16),
			(uint8_t)((rgba & 0x0000FF00) >> 8),
			(uint8_t)((rgba & 0x000000FF))
		};
	}
}