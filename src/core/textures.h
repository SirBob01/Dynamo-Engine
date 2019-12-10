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
        
        std::unordered_map<std::string, TTF_Font *> fonts_;
        std::unordered_map<std::string, SDL_Surface *> surface_map_;

    public:
        TextureManager(SDL_Renderer *renderer);
        ~TextureManager();

        // Pre-load resources on Scene initialization
        // If texture id exists, throw error
        void load_image(std::string id, std::string filename);
        void load_text(std::string id, std::string text, 
                       std::string font_id, Color color);

        // Load a TTF font file into the local database
        void load_font(std::string font_id, std::string filename, int size);

        // Unload a texture to reuse the id key
        void unload_surface(std::string id);

        // Unload a font to reuse the id key
        void unload_font(std::string font_id);

        // Get a texture from an id key
        SDL_Texture *get_texture(std::string id);

        // Free all loaded textures from memory
        void clear_all();
    };    
}

#endif