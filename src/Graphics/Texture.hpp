#pragma once

#include <vector>

#include "../Math/Vec2.hpp"
#include "../Types.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Texture image.
     *
     */
    struct Texture {
        /**
         * @brief 4-channel pixel data.
         *
         */
        std::vector<u8> pixels;

        /**
         * @brief Dimensions of the image.
         *
         */
        Vec2 size;
    };
} // namespace Dynamo::Graphics