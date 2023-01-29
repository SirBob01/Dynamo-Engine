#include "./Texture.hpp"

namespace Dynamo::Graphics {
    Texture::Texture(std::vector<u8> &pixels,
                     u32 width,
                     u32 height) :
        _pixels(pixels),
        _width(width), _height(height) {}

    u32 Texture::get_width() const { return _width; }

    u32 Texture::get_height() const { return _height; }

    const std::vector<u8> &Texture::get_pixels() const {
        return _pixels;
    }
} // namespace Dynamo::Graphics