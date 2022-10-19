#pragma once

#include <array>

namespace Dynamo {
    /**
     * @brief A 3x3 square matrix
     *
     */
    struct Mat3 {
        std::array<float, 9> values;

        /**
         * @brief Construct a new Mat3 object
         *
         */
        Mat3() : values({1, 0, 0, 0, 1, 0, 0, 0, 1}) {}

        /**
         * @brief Transpose this matrix
         *
         * @return Mat3&
         */
        inline Mat3 &transpose() {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    std::swap(*(values.data() + (i + j * 3)),
                              *(values.data() + (j + i * 3)));
                }
            }
            return *this;
        }
    };
} // namespace Dynamo