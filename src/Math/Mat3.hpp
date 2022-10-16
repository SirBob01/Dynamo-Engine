#pragma once

namespace Dynamo {
    /**
     * @brief A 3x3 square matrix
     *
     */
    struct Mat3 {
        float a, b, c;
        float d, e, f;
        float g, h, i;

        /**
         * @brief Construct a new Mat3 object
         *
         * @param a
         * @param b
         * @param c
         * @param d
         * @param e
         * @param f
         * @param g
         * @param h
         * @param i
         */
        Mat3(float a = 1,
             float b = 0,
             float c = 0,
             float d = 0,
             float e = 1,
             float f = 0,
             float g = 0,
             float h = 0,
             float i = 1) :
            a(a),
            b(b), c(c), d(d), e(e), f(f), g(g), h(h), i(i) {}
    };
} // namespace Dynamo