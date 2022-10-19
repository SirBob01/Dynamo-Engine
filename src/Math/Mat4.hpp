#pragma once

#include <algorithm>
#include <array>

namespace Dynamo {
    /**
     * @brief A 4x4 square matrix
     *
     */
    struct Mat4 {
        std::array<float, 16> values;

        /**
         * @brief Construct a new Mat4 object
         *
         */
        Mat4() : values({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}) {}

        /**
         * @brief Transpose this matrix
         *
         * @return Mat4&
         */
        inline Mat4 &transpose() {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    std::swap(*(values.data() + (i + j * 4)),
                              *(values.data() + (j + i * 4)));
                }
            }
            return *this;
        }
    };
} // namespace Dynamo