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

        Vec2D logic_dim = {
            static_cast<float>(width), 
            static_cast<float>(height)
        };
        logic_bounds_ = {
            logic_dim/2,
            logic_dim
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
        return logic_bounds_.dim;
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
        draw_rect(nullptr, logic_bounds_, color, true);
    }

    void Display::set_borderfill(Color color) {
        border_color_ = color;
    }

    void Display::set_dimensions(int width, int height) {
        Vec2D logic_dim = {
            static_cast<float>(width), 
            static_cast<float>(height)
        };
        logic_bounds_ = {
            logic_dim/2,
            logic_dim
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
        AABB aabb = {position, source->get_dimensions()};
        if(!source->get_visible() || 
           source->get_alpha() <= 0 ||
           !aabb.is_colliding(logic_bounds_)) {
            return;
        }
        SDL_SetTextureBlendMode(
            source->get_base(),
            static_cast<SDL_BlendMode>(mode)
        );

        // Render source on dest
        set_render_target(dest);
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
        if(!logic_bounds_.is_in_bounds(point)) {
            return;
        }
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
        if(!logic_bounds_.is_in_bounds(point1) && 
           !logic_bounds_.is_in_bounds(point2)) {
            return;
        }
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
        if(!box.is_colliding(logic_bounds_)) {
            return;
        }
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
        // Midpoint algorithm generates circle points
        int x = radius;
        int y = 0;
        int p = 1 - radius;

        int cx = center.x;
        int cy = center.y;

        std::vector<SDL_Point> points;
        while(x >= y) {
            points.push_back({cx + x, cy + y});
            points.push_back({cx - x, cy + y});
            points.push_back({cx + y, cy + x});
            points.push_back({cx - y, cy + x});
            points.push_back({cx + x, cy - y});
            points.push_back({cx - x, cy - y});
            points.push_back({cx + y, cy - x});
            points.push_back({cx - y, cy - x});

            if(p <= 0) {
                p += 2*y + 1;
            }
            else {
                x--;
                p += 2*y - 2*x + 1;
            }
            y++;
        }

        // Batch render the points
        SDL_SetRenderDrawBlendMode(
            renderer_,
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            renderer_, 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        if(!fill) {
            SDL_RenderDrawPoints(
                renderer_,
                &points[0],
                points.size()
            );
        }
        else {
            SDL_RenderDrawLines(
                renderer_,
                &points[0],
                points.size()
            );
        }
        set_render_target(nullptr);
    }

    void Display::draw_polygon(Sprite *dest, Vec2D points[], int n, 
                               Color color, bool fill, RENDER_BLEND mode) {
        if(!fill) {
            for(int i = 0; i < n; i++) {
                draw_line(dest, points[i], points[(i+1)%n], color, mode);
            }
        }
        else {
            float nodes_x[n]; 
            int nodes, row, i, j;
            Vec2D min = points[0], max = points[0];
            
            // Find minimum and maximum coordinates in polygon
            for(i = 0; i < n; i++) {
                if(points[i].y < min.y) {
                    min.y = points[i].y;
                }
                if(points[i].y > max.y) {
                    max.y = points[i].y;
                }

                if(points[i].x < min.x) {
                    min.x = points[i].x;
                }
                if(points[i].x > max.x) {
                    max.x = points[i].x;
                }
            }

            // Generate and draw lines between nodes
            for(row = min.y; row < max.y; row++) {
                nodes = 0;
                j = n - 1;
                for(i = 0; i < n; i++) {
                    if(points[i].y < row && points[j].y >= row ||
                       points[j].y < row && points[i].y >= row) {
                        nodes_x[nodes++] = (
                            (points[i].x + (row - points[i].y) /
                            (points[j].y - points[i].y) *
                            (points[j].x - points[i].x))
                        );
                    }
                    j = i;
                }

                for(i = 0; i < nodes - 1;) {
                    if(nodes_x[i] > nodes_x[i + 1]) {
                        std::swap(nodes_x[i], nodes_x[i + 1]);
                        if(i) {
                            i--;
                        }
                    }
                    else {
                        i++;
                    }
                }

                for(i = 0; i < nodes; i += 2) {
                    if(nodes_x[i] >= max.x) {
                        break;
                    }
                    if(nodes_x[i + 1] > min.x) {
                        if(nodes_x[i] < min.x) {
                            nodes_x[i] = min.x;
                        }
                        if(nodes_x[i + 1] > max.x) {
                            nodes_x[i + 1] = max.x;
                        }

                        Vec2D start = {
                            nodes_x[i], 
                            static_cast<float>(row)
                        };
                        Vec2D end = {
                            nodes_x[i + 1], 
                            static_cast<float>(row)
                        };
                        draw_line(dest, start, end, color, mode);
                    }
                }
            }
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