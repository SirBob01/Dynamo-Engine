#pragma once

#include <vector>

#include "../Types.hpp"
#include "./Layer.hpp"
#include "./LayerView.hpp"

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

        /**
         * @brief Get the rendering layers.
         *
         * @return const std::vector<Layer>&
         */
        virtual const std::vector<Layer> &get_layers() const = 0;

        /**
         * @brief Set the rendering layers.
         *
         * @param layers Ordered list of layer configurations
         */
        virtual void set_layers(std::vector<LayerConfiguration> layers) = 0;
    };
} // namespace Dynamo::Graphics