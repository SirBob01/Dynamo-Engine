#pragma once

#include <vector>

namespace Dynamo::Graphics {
    /**
     * @brief Model texture
     *
     */
    class Texture {
      protected:
        std::vector<u8> _pixels;
        u32 _width;
        u32 _height;

      public:
        /**
         * @brief Construct a new Texture object
         *
         * @param pixels 4-channel pixel data
         * @param width  Width of the texture image
         * @param height Height of the texture image
         */
        Texture(std::vector<u8> &pixels,
                u32 width,
                u32 height);

        /**
         * @brief Destroy the Texture object
         *
         */
        virtual ~Texture() = default;

        /**
         * @brief Get the width of the texture
         *
         * @return u32
         */
        u32 get_width() const;

        /**
         * @brief Get the height of the texture
         *
         * @return u32
         */
        u32 get_height() const;

        /**
         * @brief Get the pixels object
         *
         * @return const std::vector<u8>&
         */
        const std::vector<u8> &get_pixels() const;
    };
} // namespace Dynamo::Graphics