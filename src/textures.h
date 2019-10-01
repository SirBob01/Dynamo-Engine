#ifndef DYNAMO_TEXTURES_H_
#define DYNAMO_TEXTURES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <unordered_map>
#include <string>

#include "vec2d.h"
#include "color.h"
#include "error.h"

namespace Dynamo {
    struct Texture {
        SDL_Texture *texture;
        SDL_TextureAccess access;
    };

    class TextureManager {
        SDL_Renderer *renderer_;
        
        std::unordered_map<std::string, TTF_Font *> fonts_;
        std::unordered_map<std::string, Texture> texture_map_;

    public:
        TextureManager(SDL_Renderer *renderer);
        ~TextureManager();

        // Pre-load resources on Scene initialization
        // If texture id exists, it is overwritten to prevent memory leaks
        void load_surface(std::string id, Vec2D dimensions);
        void load_image(std::string id, std::string filename);
        void load_text(std::string id, std::string text, 
                       std::string font_id, Color color);

        // Load a TTF font file into the local database
        void load_font(std::string font_id, std::string filename, int size);

        // Get a texture from an id key
        Texture &get_texture(std::string id);

        // Free all loaded textures from memory
        void clear_all();
    };    
}

#endif