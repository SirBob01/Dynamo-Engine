#ifndef DYNAMO_TEXTURES_H_
#define DYNAMO_TEXTURES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <unordered_map>
#include <string>

#include "../util/color.h"
#include "../util/vec2d.h"
#include "../log/error.h"

namespace Dynamo {
    class TextureManager {
        SDL_Renderer *renderer_;

        std::string default_image_path_;
        std::string default_font_path_;

        std::unordered_map<std::string, TTF_Font *> fonts_;
        std::unordered_map<std::string, SDL_Texture *> texture_map_;

        // Convert a surface to a texture
        SDL_Texture *convert_surface(SDL_Surface *surface);

    public:
        TextureManager(SDL_Renderer *renderer);
        ~TextureManager();

        // Load an image texture
        SDL_Texture *load_image(std::string filename);

        // Load a renderable-text texture
        SDL_Texture *load_text(std::string text, 
                               std::string font_id, Color color);

        // Load a TTF font file
        void load_font(std::string font_id, std::string filename, int size);

        // Set the default path for images
        void set_image_path(std::string path);

        // Set the default path for fonts
        void set_font_path(std::string path);

        // Free all loaded textures from memory
        void clear_textures();

        // Free all loaded fonts from memory
        void clear_fonts();
    };    
}

#endif