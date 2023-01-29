#pragma once

#include <array>

#include "../Types.hpp"
#include "./Common.hpp"

namespace Dynamo {
    /**
     * @brief RGBA color value
     *
     * Each channel accepts a floating point number from [0 - 1], values outside
     * this range are invalid
     *
     */
    struct Color {
        f32 r;
        f32 g;
        f32 b;
        f32 a;

        /**
         * @brief Construct a new Color object
         *
         * @param r Red channel
         * @param g Green channel
         * @param b Blue channel
         * @param a Alpha channel
         */
        constexpr Color(f32 r = 0, f32 g = 0, f32 b = 0, f32 a = 1) :
            r(r), g(g), b(b), a(a) {}

        /**
         * @brief Linearly interpolate between two colors
         *
         * @param rhs Target color
         * @param t   Interpolation factor
         * @return Color
         */
        Color lerp(const Color &rhs, f32 t) const {
            return Color(Dynamo::lerp(r, rhs.r, t),
                         Dynamo::lerp(g, rhs.g, t),
                         Dynamo::lerp(b, rhs.b, t),
                         Dynamo::lerp(a, rhs.a, t));
        }

        /**
         * @brief Invert the RGB channels, maintaining the alpha channel
         *
         * @return Color
         */
        Color invert() { return Color(1 - r, 1 - g, 1 - b, a); }

        /**
         * @brief Convert the color to a single 32-bit u32 integer which
         * can be represented as a hex value
         *
         * @return constexpr u32
         */
        constexpr u32 to_hex() const {
            u32 r_v = UINT8_MAX * r;
            u32 g_v = UINT8_MAX * g;
            u32 b_v = UINT8_MAX * b;
            u32 a_v = UINT8_MAX * a;
            return (r_v << 24) | (g_v << 16) | (b_v << 8) | (a_v);
        }

        /**
         * @brief Convert the Color into an array of floats
         *
         * @return std::array<f32, 4>
         */
        std::array<f32, 4> to_array() const { return {r, g, b, a}; }
    };
} // namespace Dynamo