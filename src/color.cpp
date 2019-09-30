#include "color.h"

namespace Dynamo {
	uint32_t Color::get_hex() {
		return r << 24 | g << 16 | b << 8 | a;
	}

	Color &Color::scale(float scalar) {
		r = (uint8_t)(r * scalar);
		g = (uint8_t)(g * scalar);
		b = (uint8_t)(b * scalar);
		a = (uint8_t)(a * scalar);
		clamp();
		return *this;
	}

	Color &Color::clamp() {
		r = std::max((uint8_t)0, std::min(r, (uint8_t)255));
		g = std::max((uint8_t)0, std::min(g, (uint8_t)255));
		b = std::max((uint8_t)0, std::min(b, (uint8_t)255));
		a = std::max((uint8_t)0, std::min(a, (uint8_t)255));
		return *this;
	}

	const Color Color::lerp(const Color &other, float rate) const {
		uint8_t new_r = r + (other.r - r) * rate;
		uint8_t new_g = g + (other.g - g) * rate;
		uint8_t new_b = b + (other.b - b) * rate;
		uint8_t new_a = a + (other.a - a) * rate;
		return {new_r, new_g, new_b, new_a};
	}

	Color hex_color(uint32_t rgba) {
		return {
			(uint8_t)((rgba & 0xFF000000) >> 24), 
			(uint8_t)((rgba & 0x00FF0000) >> 16),
			(uint8_t)((rgba & 0x0000FF00) >> 8),
			(uint8_t)((rgba & 0x000000FF))
		};
	}
}