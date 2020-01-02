#include "display.h"

namespace Dynamo {
    Display::Display(int width, int height, 
                     std::string title, bool fullscreen) {
        SDL_DisplayMode dm;
        if(SDL_GetDesktopDisplayMode(0, &dm)) {
            throw SDLError(SDL_GetError());
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

        border_color_ = {0, 0, 0};

        renderer_ = SDL_CreateRenderer(
            window_, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
        );
        SDL_RenderSetLogicalSize(renderer_, width, height);
    }

    Display::~Display() {
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
    }

    void Display::set_render_target(Sprite *dest) {
        if(dest) {
            // Blend rendering over target sprite
            SDL_SetRenderTarget(renderer_, dest->get_base());
        }
        else {
            // Default render target is the main display
            SDL_SetRenderTarget(renderer_, nullptr);
        }
    }

    bool Display::is_fullscreen() {
        return SDL_GetWindowFlags(window_) & SDL_WINDOW_FULLSCREEN;
    }

    SDL_Window *Display::get_window() {
        return window_;
    }

    SDL_Renderer *Display::get_renderer() {
        return renderer_;
    }

    Vec2D Display::get_dimensions() {
        return logic_dim_;
    }

    Vec2D Display::get_window_dimensions() {
        SDL_GetWindowSize(window_, &window_x_, &window_y_);
        return {
            static_cast<float>(window_x_), 
            static_cast<float>(window_y_)
        };
    }

    Color Display::get_borderfill() {
        return border_color_;
    }

    void Display::set_title(std::string title) {
        SDL_SetWindowTitle(window_, title.c_str());
    }

    void Display::set_fill(Color color) {
        draw_rect(nullptr, {get_dimensions()/2, logic_dim_}, color, true);
    }

    void Display::set_borderfill(Color color) {
        border_color_ = color;
    }

    void Display::set_dimensions(int width, int height) {
        logic_dim_ = {
            static_cast<float>(width), 
            static_cast<float>(height)
        };
        SDL_RenderSetLogicalSize(renderer_, width, height);
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

    void Display::draw_sprite(Sprite *dest, Sprite *source, Vec2D position,
                              RENDER_BLEND mode) {
        if(!source->get_visible() || source->get_alpha() <= 0) {
            return;
        }
        SDL_SetTextureBlendMode(
            source->get_base(),
            static_cast<SDL_BlendMode>(mode)
        );

        // Render source on dest
        set_render_target(dest);
        AABB aabb = {position, source->get_dimensions()};
        SDL_Rect target_rect = aabb.convert_to_rect();
        SDL_RenderCopyEx(
            renderer_,
            source->get_base(),
            nullptr,
            &target_rect,
            source->get_angle(),
            nullptr,
            source->get_flip()
        );

        // Render actual source texture on its base
        set_render_target(source);
        SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 0);
        SDL_RenderClear(renderer_);
        SDL_RenderCopy(
            renderer_, 
            source->get_texture(), 
            source->get_source(), 
            nullptr
        );
        set_render_target(nullptr);
    }

    void Display::draw_point(Sprite *dest, Vec2D point, 
                             Color color, RENDER_BLEND mode) {
        SDL_SetRenderDrawBlendMode(
            renderer_,
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        SDL_Point sdl_point = point.convert_to_point();
        SDL_RenderDrawPoint(
            renderer_, 
            sdl_point.x,
            sdl_point.y
        );
        set_render_target(nullptr);
    }

    void Display::draw_line(Sprite *dest, Vec2D point1, Vec2D point2, 
                            Color color, RENDER_BLEND mode) {
        SDL_SetRenderDrawBlendMode(
            renderer_,
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        SDL_Point sdl_p1 = point1.convert_to_point();
        SDL_Point sdl_p2 = point2.convert_to_point();
        SDL_RenderDrawLine(
            renderer_, 
            sdl_p1.x,
            sdl_p1.y, 
            sdl_p2.x,
            sdl_p2.y
        );
        set_render_target(nullptr);
    }

    void Display::draw_rect(Sprite *dest, AABB box, 
                            Color color, bool fill, RENDER_BLEND mode) {
        SDL_SetRenderDrawBlendMode(
            renderer_,
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        SDL_Rect rect = box.convert_to_rect();
        if(fill) {
            SDL_RenderFillRect(renderer_, &rect);
        }
        else {
            SDL_RenderDrawRect(renderer_, &rect);
        }
        set_render_target(nullptr);
    }

    void Display::draw_circle(Sprite *dest, Vec2D center, int radius, 
                              Color color, bool fill, RENDER_BLEND mode) {
        // Midpoint algorithm
        int x = radius;
        int y = 0;
        int p = 1 - radius;

        float cx = center.x;
        float cy = center.y;

        while(x >= y) {
            if(!fill) {
                draw_point(dest, {cx + x, cy + y}, color, mode);
                draw_point(dest, {cx + x, cy - y}, color, mode);
                draw_point(dest, {cx - x, cy + y}, color, mode);
                draw_point(dest, {cx - x, cy - y}, color, mode);
                draw_point(dest, {cx + y, cy + x}, color, mode);
                draw_point(dest, {cx + y, cy - x}, color, mode);
                draw_point(dest, {cx - y, cy + x}, color, mode);
                draw_point(dest, {cx - y, cy - x}, color, mode);
            }
            else {
                draw_line(
                    dest,
                    {cx + x, cy + y}, {cx - x, cy + y}, 
                    color, mode
                );
                draw_line(
                    dest,
                    {cx + y, cy + x}, {cx - y, cy + x}, 
                    color, mode
                );
                draw_line(
                    dest,
                    {cx + x, cy - y}, {cx - x, cy - y}, 
                    color, mode
                );
                draw_line(
                    dest,
                    {cx + y, cy - x}, {cx - y, cy - x}, 
                    color, mode
                );
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

    void Display::draw_polygon(Sprite *dest, Vec2D points[], int n, 
                               Color color, RENDER_BLEND mode) {
        for(int i = 1; i < n; i++) {
            draw_line(dest, points[i-1], points[i], color, mode);
        }
    }

    void Display::refresh() {
        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(
            renderer_, 
            border_color_.r, border_color_.g, border_color_.b, 0xFF
        );
        SDL_RenderPresent(renderer_);
        SDL_RenderClear(renderer_);
    }
}