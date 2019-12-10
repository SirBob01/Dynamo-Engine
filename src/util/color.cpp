#include "color.h"

namespace Dynamo {
    uint32_t Color::get_hex() {
        uint8_t u_r = r;
        uint8_t u_g = g;
        uint8_t u_b = b;
        uint8_t u_a = a;
        return u_r << 24 | u_g << 16 | u_b << 8 | u_a;
    }

    Color &Color::scale(float scalar) {
        r = static_cast<int>(r * scalar);
        g = static_cast<int>(g * scalar);
        b = static_cast<int>(b * scalar);
        a = static_cast<int>(a * scalar);
        clamp();
        return *this;
    }

    Color &Color::clamp() {
        r = std::max(0, std::min(r, 255));
        g = std::max(0, std::min(g, 255));
        b = std::max(0, std::min(b, 255));
        a = std::max(0, std::min(a, 255));
        return *this;
    }

    const Color Color::lerp(const Color &other, float rate) const {
        int new_r = r + (other.r - r) * rate;
        int new_g = g + (other.g - g) * rate;
        int new_b = b + (other.b - b) * rate;
        int new_a = a + (other.a - a) * rate;
        return {new_r, new_g, new_b, new_a};
    }

    Color hex_color(uint32_t rgba) {
        return {
            static_cast<int>((rgba & 0xFF000000) >> 24), 
            static_cast<int>((rgba & 0x00FF0000) >> 16),
            static_cast<int>((rgba & 0x0000FF00) >> 8),
            static_cast<int>((rgba & 0x000000FF))
        };
    }
}