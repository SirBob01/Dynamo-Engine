#include "display.h"

namespace Dynamo {
    Display::Display(int width, int height, 
                     std::string title, bool fullscreen) {
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
        
        window_ = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE
        );
    }

    Display::~Display() {
        SDL_DestroyWindow(window_);
    }

    bool Display::is_fullscreen() {
        return SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN;
    }

    SDL_Window *Display::get_window() {
        return window_;
    }

    Vec2D Display::get_window_dimensions() {
        int window_x, window_y;
        SDL_GetWindowSize(window_, &window_x, &window_y);
        return {
            static_cast<float>(window_x), 
            static_cast<float>(window_y)
        };
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