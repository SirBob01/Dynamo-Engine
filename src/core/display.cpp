#include "display.h"

namespace Dynamo {
    Display::Display(int width, int height, std::string title, 
                     bool fullscreen, bool vsync) {
        SDL_DisplayMode dm;
        if(SDL_GetDesktopDisplayMode(0, &dm)) {
            throw SDLError();
        }

        if(!width) {
            width = fullscreen ? dm.w : dm.w * 0.75;
        }
        if(!height) {
            height = fullscreen ? dm.h : dm.h * 0.75;
        }
        logic_dim_ = {
            static_cast<float>(width),
            static_cast<float>(height)
        };
        
        window_ = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE
        );

        int flags = SDL_RENDERER_ACCELERATED;
        if(vsync) {
            flags = flags | SDL_RENDERER_PRESENTVSYNC;
        }
        renderer_ = SDL_CreateRenderer(
            window_, 
            -1,
            flags
        );
        set_dimensions(width, height);
    }

    Display::~Display() {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
    }

    SDL_Window *Display::get_window() {
        return window_;
    }

    SDL_Renderer *Display::get_renderer() {
        return renderer_;
    }

    bool Display::is_fullscreen() {
        return SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN;
    }

    Vec2D Display::get_window_dimensions() {
        int window_x, window_y;
        SDL_GetWindowSize(window_, &window_x, &window_y);
        return {
            static_cast<float>(window_x), 
            static_cast<float>(window_y)
        };
    }
    
    Vec2D Display::get_dimensions() {
        return logic_dim_;
    }

    void Display::set_dimensions(int width, int height) {
        logic_dim_.x = width;
        logic_dim_.y = height;
        SDL_RenderSetLogicalSize(renderer_, width, height);
    }

    void Display::set_title(std::string title) {
        SDL_SetWindowTitle(window_, title.c_str());
    }

    void Display::set_fullscreen(bool mode) {
        if(is_fullscreen() && mode) {
            return;
        }
        SDL_SetWindowFullscreen(
            window_, 
            mode ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE
        );
    }

    void Display::set_cursor_visibility(bool mode) {
        SDL_ShowCursor(mode);
    }
}