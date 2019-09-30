#ifndef DYNAMO_COLOR_H_
#define DYNAMO_COLOR_H_

#include <SDL2/SDL.h>

#include <algorithm>

namespace Dynamo {
    struct Color {
        uint8_t r, g, b; 
        uint8_t a = 255; // Default alpha value

        // Convert color to hexidecimal
        uint32_t get_hex();

        // Scale components
        Color &scale(float scalar);

        // Restrict values to [0, 255]
        Color &clamp();

        // Interpolate between colors
        const Color lerp(const Color &other, float rate) const;
    };

    Color hex_color(uint32_t rgba);
}


#endif