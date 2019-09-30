#include "display.h"

namespace Dynamo {
    Display::Display(int width, int height, std::string title) {
        logic_w_ = width;
        logic_h_ = height;

        window_ = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_RESIZABLE
        );

        border_color_ = {0, 0, 0};

        renderer_ = SDL_CreateRenderer(
            window_, 
            -1, 
            SDL_RENDERER_ACCELERATED
        );
        SDL_RenderSetLogicalSize(renderer_, width, height);
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

    int Display::get_width() {
        return logic_w_;
    }

    int Display::get_height() {
        return logic_h_;
    }

    int Display::get_window_width() {
        SDL_GetWindowSize(window_, &window_w_, nullptr);
        return window_w_;
    }

    int Display::get_window_height() {
        SDL_GetWindowSize(window_, nullptr, &window_h_);
        return window_h_;
    }

    void Display::set_title(std::string title) {
        SDL_SetWindowTitle(window_, title.c_str());
    }

    void Display::set_fill(Color color) {
        draw_rect(0, 0, logic_w_, logic_h_, color, true);
    }

    void Display::set_borderfill(Color color) {
        border_color_ = color;
    }

    void Display::draw_sprite(Sprite *sprite) {
        if(sprite->get_visible() && sprite->get_alpha()) {
            SDL_RenderCopyEx(
                renderer_,
                sprite->get_texture(),
                sprite->get_source(),
                sprite->get_target(),
                sprite->get_angle(),
                nullptr,
                sprite->get_flip()
            );
        }
    }

    void Display::draw_point(int x, int y, Color color) {
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );
        SDL_RenderDrawPoint(renderer_, x, y);
    }

    void Display::draw_line(int x1, int y1, 
                            int x2, int y2, 
                            Color color) {
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );
        SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
    }

    void Display::draw_rect(int x, int y, int w, int h, 
                            Color color, bool fill) {
        SDL_Rect rect = {x, y, w, h};
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );
        if(fill) {
            SDL_RenderFillRect(renderer_, &rect);
        }
        else {
            SDL_RenderDrawRect(renderer_, &rect);
        }
    }

    void Display::draw_circle(int cx, int cy, int r, 
                              Color color, bool fill) {
        // Midpoint algorithm
        int x = r;
        int y = 0;
        int p = 1 - r;

        while(x >= y) {
            if(!fill) {
                draw_point(cx+x, cy+y, color);
                draw_point(cx+x, cy-y, color);
                draw_point(cx-x, cy+y, color);
                draw_point(cx-x, cy-y, color);
                draw_point(cx+y, cy+x, color);
                draw_point(cx+y, cy-x, color);
                draw_point(cx-y, cy+x, color);
                draw_point(cx-y, cy-x, color);
            }
            else {
                draw_line(cx+x, cy+y, cx-x, cy+y, color);
                draw_line(cx+y, cy+x, cx-y, cy+x, color);
                draw_line(cx+x, cy-y, cx-x, cy-y, color);
                draw_line(cx+y, cy-x, cx-y, cy-x, color);
            }
            if(p <= 0) {
                p += 2*y + 1;
            }
            else {
                x--;
                p += 2*y - 2*x + 1;
            }
            y++;
        }
    }

    void Display::refresh() {
        SDL_SetRenderDrawColor(
            renderer_, 
            border_color_.r, border_color_.g, border_color_.b, 0xFF
        );
        SDL_RenderPresent(renderer_);
        SDL_RenderClear(renderer_);
    }
}