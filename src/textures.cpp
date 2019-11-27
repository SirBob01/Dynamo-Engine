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
        
        if(surface_map_.count(id)) {
            throw ValueExists(id, "surface_map_");
        }
        
        // Colormask is byte order dependent
        uint32_t rmask, gmask, bmask, amask;
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        #else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
        #endif

        // Create a colorless, blank canvas
        SDL_Surface *surf = SDL_CreateRGBSurface(
            0,
            width, height,
            32,
            rmask, gmask, bmask, amask
        );
        surface_map_[id] = surf;
    }

    void TextureManager::load_image(std::string id, std::string filename) {
        if(surface_map_.count(id)) {
            throw ValueExists(id, "surface_map_");
        }

        SDL_Surface *surf = IMG_Load(filename.c_str());
        surface_map_[id] = surf;
    }

    void TextureManager::load_text(std::string id, std::string text, 
                             std::string font_id, Color color) {
        if(surface_map_.count(id)) {
            throw ValueExists(id, "surface_map_");
        }
        if(!fonts_.count(font_id)) {
            throw InvalidKey(font_id, "fonts_");
        }

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
        surface_map_[id] = surf;
    }

    void TextureManager::load_font(std::string font_id, 
                             std::string filename, int size) {
        if(fonts_.count(font_id)) {
            throw ValueExists(font_id, "fonts_");
        }

        fonts_[font_id] = TTF_OpenFont(filename.c_str(), size);
    }

    void TextureManager::unload_surface(std::string id) {
        if(!surface_map_.count(id)) {
            throw InvalidKey(id, "surface_map_");
        }
        SDL_FreeSurface(surface_map_[id]);
        surface_map_.erase(id);
    }

    void TextureManager::unload_font(std::string font_id) {
        if(!fonts_.count(font_id)) {
            throw InvalidKey(font_id, "fonts_");
        }
        TTF_CloseFont(fonts_[font_id]);
        fonts_.erase(font_id);
    }

    SDL_Texture *TextureManager::get_texture(std::string id) {
        if(!surface_map_.count(id)) {
            throw InvalidKey(id, "surface_map_");
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(
            renderer_, 
            surface_map_[id]
        );
        return texture;
    }

    void TextureManager::clear_all() {
        for(auto &surface : surface_map_) {
            SDL_FreeSurface(surface.second);
        }
        for(auto &font : fonts_) {
            TTF_CloseFont(font.second);
        }
        surface_map_.clear();
        fonts_.clear();
    }
}