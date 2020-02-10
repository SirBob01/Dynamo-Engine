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
        r = Util::clamp(r, 0, 255);
        g = Util::clamp(g, 0, 255);
        b = Util::clamp(b, 0, 255);
        a = Util::clamp(a, 0, 255);
        return *this;
    }

    const Color Color::lerp(const Color &other, float rate) const {
        int new_r = Util::lerp(r, other.r, rate);
        int new_g = Util::lerp(g, other.g, rate);
        int new_b = Util::lerp(b, other.b, rate);
        int new_a = Util::lerp(a, other.a, rate);
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