#include "display.h"

namespace Dynamo {
    Display::Display(int width, int height, std::string title) {
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

    void Display::set_title(std::string title) {
        SDL_SetWindowTitle(window_, title.c_str());
    }

    void Display::set_fill(Color color) {
        draw_rect({{0, 0}, logic_dim_}, color, true, false);
    }

    void Display::set_borderfill(Color color) {
        border_color_ = color;
    }

    void Display::draw_sprite(Sprite *sprite, Vec2D position, bool center) {
        if(!sprite->get_visible() || sprite->get_alpha() <= 0) {
            return;
        }

        Vec2D dimensions = sprite->get_dimensions();
        Vec2D ref_point;
        if(center) {
            ref_point = position - dimensions/2;
        }
        else {
            ref_point = position;
        }

        SDL_Rect target = {
            static_cast<int>(ref_point.x),
            static_cast<int>(ref_point.y),
            static_cast<int>(dimensions.x),
            static_cast<int>(dimensions.y)
        };

        SDL_RenderCopyEx(
            renderer_,
            sprite->get_texture(),
            sprite->get_source(),
            &target,
            sprite->get_angle(),
            nullptr,
            sprite->get_flip()
        );
    }

    void Display::draw_point(Vec2D point, Color color) {
        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );
        SDL_RenderDrawPoint(
            renderer_, 
            static_cast<int>(point.x), static_cast<int>(point.y)
        );
    }

    void Display::draw_line(Vec2D point1, Vec2D point2, 
                            Color color) {
        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );
        SDL_RenderDrawLine(
            renderer_, 
            static_cast<int>(point1.x), static_cast<int>(point1.y), 
            static_cast<int>(point2.x), static_cast<int>(point2.y)
        );
    }

    void Display::draw_rect(AABB box, Color color, bool fill, bool center) {
        Vec2D min;
        if(center) {
            min = box.get_min();
        }
        else {
            min = box.center;
        }
        SDL_Rect rect = {
            static_cast<int>(min.x), static_cast<int>(min.y), 
            static_cast<int>(box.dim.x), static_cast<int>(box.dim.y)
        };

        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
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

    void Display::draw_circle(Vec2D center, int radius, 
                              Color color, bool fill) {
        // Midpoint algorithm
        int x = radius;
        int y = 0;
        int p = 1 - radius;

        float cx = center.x;
        float cy = center.y;

        while(x >= y) {
            if(!fill) {
                draw_point({cx + x, cy + y}, color);
                draw_point({cx + x, cy - y}, color);
                draw_point({cx - x, cy + y}, color);
                draw_point({cx - x, cy - y}, color);
                draw_point({cx + y, cy + x}, color);
                draw_point({cx + y, cy - x}, color);
                draw_point({cx - y, cy + x}, color);
                draw_point({cx - y, cy - x}, color);
            }
            else {
                draw_line({cx + x, cy + y}, {cx - x, cy + y}, color);
                draw_line({cx + y, cy + x}, {cx - y, cy + x}, color);
                draw_line({cx + x, cy - y}, {cx - x, cy - y}, color);
                draw_line({cx + y, cy - x}, {cx - y, cy - x}, color);
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
        SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(
            renderer_, 
            border_color_.r, border_color_.g, border_color_.b, 0xFF
        );
        SDL_RenderPresent(renderer_);
        SDL_RenderClear(renderer_);
    }
}