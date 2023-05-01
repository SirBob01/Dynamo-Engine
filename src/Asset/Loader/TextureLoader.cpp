#include "./TextureLoader.hpp"

namespace Dynamo {
    Graphics::Texture TextureLoader::load(const std::string &filepath) {
        i32 width, height, channels;
        stbi_uc *raw = stbi_load(filepath.c_str(),
                                 &width,
                                 &height,
                                 &channels,
                                 STBI_rgb_alpha);
        if (raw == nullptr) {
            Log::error("Could not load image file: {}", filepath);
        }

        Graphics::Texture texture;
        texture.size.x = width;
        texture.size.y = height;

        texture.pixels.resize(width * height * STBI_rgb_alpha);
        std::copy(raw, raw + texture.pixels.size(), texture.pixels.data());
        stbi_image_free(raw);

        return texture;
    }
} // namespace Dynamo