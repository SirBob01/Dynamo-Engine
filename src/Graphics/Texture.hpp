#pragma once

#include <vector>

#include "../Types.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Model texture.
     *
     */
    class Texture {
      protected:
        std::vector<u8> _pixels;
        u32 _width;
        u32 _height;

      public:
        /**
         * @brief Construct a new Texture object.
         *
         * @param pixels 4-channel pixel data.
         * @param width  Width of the texture image.
         * @param height Height of the texture image.
         */
        Texture(std::vector<u8> &pixels, u32 width, u32 height);

        /**
         * @brief Destroy the Texture object.
         *
         */
        virtual ~Texture() = default;

        /**
         * @brief Get the width of the texture.
         *
         * @return u32
         */
        inline u32 get_width() const { return _width; }

        /**
         * @brief Get the height of the texture.
         *
         * @return u32
         */
        inline u32 get_height() const { return _height; }

        /**
         * @brief Get the pixels object.
         *
         * @return const std::vector<u8>&
         */
        inline const std::vector<u8> &get_pixels() const { return _pixels; }
    };
} // namespace Dynamo::Graphics