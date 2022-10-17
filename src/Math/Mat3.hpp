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
         */
        Mat3() {}
    };
} // namespace Dynamo