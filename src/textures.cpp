#include "textures.h"

namespace Dynamo {
    Textures::Textures(SDL_Renderer *renderer) {
        renderer_ = renderer;
        TTF_Init();
    }

    Textures::~Textures() {
        clear_all();
        TTF_Quit();
    }

    void Textures::load_surface(std::string id, int width, int height) {
        if(texture_map_.count(id)) {
            SDL_DestroyTexture(texture_map_[id]);
        }

        SDL_Surface *surface;
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            surface = SDL_CreateRGBSurface(
                0, width, height, 32, 
                0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF
            );
        }
        else {
            surface = SDL_CreateRGBSurface(
                0, width, height, 32, 
                0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000
            );
        }
        
        texture_map_[id] = SDL_CreateTextureFromSurface(
            renderer_, 
            surface
        );
        SDL_FreeSurface(surface);
    }

    void Textures::load_image(std::string id, std::string filename) {
        if(texture_map_.count(id)) {
            SDL_DestroyTexture(texture_map_[id]);
        }

        texture_map_[id] = IMG_LoadTexture(renderer_, filename.c_str());
    }

    void Textures::load_text(std::string id, std::string text, 
                             std::string font_id, Color color) {
        if(texture_map_.count(id)) {
            SDL_DestroyTexture(texture_map_[id]);
        }

        SDL_Surface *surf = TTF_RenderText_Solid(
            fonts_[font_id], 
            text.c_str(), 
            {color.r, color.g, color.b, color.a}
        );
        texture_map_[id] = SDL_CreateTextureFromSurface(
            renderer_, 
            surf
        );
        SDL_FreeSurface(surf);
    }

    void Textures::load_font(std::string font_id, 
                             std::string filename, int size) {
        if(fonts_.count(font_id)) {
            TTF_CloseFont(fonts_[font_id]);
        }

        fonts_[font_id] = TTF_OpenFont(filename.c_str(), size);
    }

    SDL_Texture *Textures::get_texture(std::string id) {
        return texture_map_[id];
    }

    void Textures::clear_all() {
        for(auto &item : texture_map_) {
            SDL_DestroyTexture(item.second);
        }
        for(auto &font : fonts_) {
            TTF_CloseFont(font.second);
        }
        texture_map_.clear();
        fonts_.clear();
    }
}