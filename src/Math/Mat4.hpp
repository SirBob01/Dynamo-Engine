#pragma once

namespace Dynamo {
    /**
     * @brief A 4x4 square matrix
     *
     */
    struct Mat4 {
        float a, b, c, d;
        float e, f, g, h;
        float i, j, k, l;
        float m, n, o, p;

        /**
         * @brief Construct a new Mat4 object
         */
        Mat4() {}
    };
} // namespace Dynamo