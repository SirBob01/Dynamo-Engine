#include "textures.h"

namespace Dynamo {
    SDL_Texture *TextureManager::convert_surface(SDL_Surface *surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(
            renderer_, 
            surface
        );
        if(!texture) {
            throw SDLError(SDL_GetError());
        }
        return texture;
    }

    TextureManager::TextureManager(SDL_Renderer *renderer) {
        renderer_ = renderer;

        default_image_path_ = "";
        default_font_path_ = "";

        TTF_Init();
    }

    TextureManager::~TextureManager() {
        clear_textures();
        clear_fonts();
        TTF_Quit();
    }

    Texture TextureManager::load_image(std::string filename) {
        std::string full_path = default_image_path_+filename;
        if(!texture_map_.count(full_path)) {
            SDL_Surface *surf = IMG_Load(filename.c_str());
            if(!surf) {
                throw SDLError(SDL_GetError());
            }
            texture_map_[full_path] = convert_surface(surf);
            SDL_FreeSurface(surf);
        }

        return {renderer_, texture_map_[full_path]};
    }

    Texture TextureManager::load_text(std::string text, 
                                           std::string font_id, Color color) {
        if(!fonts_.count(font_id)) {
            throw InvalidKey(font_id, "fonts_");
        }
        
        // Unique combination of text, font, and color
        std::string id = text+" "+font_id+std::to_string(color.get_hex());

        if(!texture_map_.count(id)) {
            color.clamp();
            uint8_t r = color.r;
            uint8_t g = color.g;
            uint8_t b = color.b;
            uint8_t a = color.a;

            SDL_Surface *surf = TTF_RenderText_Solid(
                fonts_[font_id], 
                text.c_str(),
                {r, g, b, a}
            );
            if(!surf) {
                throw SDLError(SDL_GetError());
            }
            texture_map_[id] = convert_surface(surf);
            SDL_FreeSurface(surf);
        }
        return {renderer_, texture_map_[id]};
    }

    void TextureManager::load_font(std::string font_id, 
                                   std::string filename, int size) {
        if(fonts_.count(font_id)) {
            return;
        }
        std::string full_path = default_font_path_ + filename;
        TTF_Font *font = TTF_OpenFont(full_path.c_str(), size);
        if(!font) {
            throw SDLError(SDL_GetError());
        }
        fonts_[font_id] = font;
    }

    void TextureManager::set_image_path(std::string path) {
        default_image_path_ = path + "/";
    }

    void TextureManager::set_font_path(std::string path) {
        default_font_path_ = path + "/";
    }

    void TextureManager::clear_textures() {
        for(auto &texture : texture_map_) {
            SDL_DestroyTexture(texture.second);
        }
        texture_map_.clear();
    }

    void TextureManager::clear_fonts() {
        for(auto &font : fonts_) {
            TTF_CloseFont(font.second);
        }
        fonts_.clear();
    }
}