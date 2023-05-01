#pragma once

#include <string>

#include <stb_image.h>

#include "../../Graphics/Texture.hpp"
#include "../../Log/Log.hpp"

namespace Dynamo {
    /**
     * @brief Texture loader.
     *
     */
    class TextureLoader {
      public:
        /**
         * @brief Load a texture from a file.
         *
         * @param filepath
         * @return Graphics::Texture
         */
        static Graphics::Texture load(const std::string &filepath);
    };
} // namespace Dynamo