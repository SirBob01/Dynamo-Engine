#include "textures.h"

namespace Dynamo {
    TextureManager::TextureManager(SDL_Renderer *renderer) {
        renderer_ = renderer;
        TTF_Init();
    }

    TextureManager::~TextureManager() {
        clear_all();
        TTF_Quit();
    }

    void TextureManager::load_surface(std::string id, Vec2D dimensions) {
        int width = dimensions.x;
        int height = dimensions.y;
        
        if(texture_map_.count(id)) {
            SDL_DestroyTexture(texture_map_[id].texture);
        }
        
        // Create a colorless, blank canvas texture
        SDL_Texture *texture = SDL_CreateTexture(
            renderer_, 
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_TARGET,
            width, height
        );
        texture_map_[id] = {texture, SDL_TEXTUREACCESS_TARGET};
    }

    void TextureManager::load_image(std::string id, std::string filename) {
        if(texture_map_.count(id)) {
            SDL_DestroyTexture(texture_map_[id].texture);
        }

        SDL_Texture *texture = IMG_LoadTexture(renderer_, filename.c_str());
        texture_map_[id] = {texture, SDL_TEXTUREACCESS_STATIC};
    }

    void TextureManager::load_text(std::string id, std::string text, 
                             std::string font_id, Color color) {
        if(texture_map_.count(id)) {
            SDL_DestroyTexture(texture_map_[id].texture);
        }

        SDL_Surface *surf = TTF_RenderText_Solid(
            fonts_[font_id], 
            text.c_str(), 
            {color.r, color.g, color.b, color.a}
        );
        SDL_Texture *texture = SDL_CreateTextureFromSurface(
            renderer_, 
            surf
        );
        texture_map_[id] = {texture, SDL_TEXTUREACCESS_STATIC};
        SDL_FreeSurface(surf);
    }

    void TextureManager::load_font(std::string font_id, 
                             std::string filename, int size) {
        if(fonts_.count(font_id)) {
            TTF_CloseFont(fonts_[font_id]);
        }

        fonts_[font_id] = TTF_OpenFont(filename.c_str(), size);
    }

    Texture &TextureManager::get_texture(std::string id) {
        if(!texture_map_.count(id)) {
            throw InvalidKey(id, "texture_map_");
        }
        return texture_map_[id];
    }

    void TextureManager::clear_all() {
        for(auto &item : texture_map_) {
            SDL_DestroyTexture(item.second.texture);
        }
        for(auto &font : fonts_) {
            TTF_CloseFont(font.second);
        }
        texture_map_.clear();
        fonts_.clear();
    }
}