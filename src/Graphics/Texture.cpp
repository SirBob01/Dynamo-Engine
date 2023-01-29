#include "./Texture.hpp"

namespace Dynamo::Graphics {
    Texture::Texture(std::vector<u8> &pixels, u32 width, u32 height) :
        _pixels(pixels), _width(width), _height(height) {}
} // namespace Dynamo::Graphics