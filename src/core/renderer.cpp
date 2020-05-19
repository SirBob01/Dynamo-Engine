#include "renderer.h"

namespace Dynamo {
    Renderer::Renderer(Display *display, bool vsync) {
        display_ = display;
        border_color_ = {0, 0, 0};
        fill_ = {0, 0, 0};
    }

    void Renderer::set_render_target(Sprite *dest) {
        if(dest) {
            // Blend rendering over target sprite
            SDL_SetRenderTarget(display_->get_renderer(), dest->get_base());
        }
        else {
            // Default render target is the main display
            SDL_SetRenderTarget(display_->get_renderer(), nullptr);
        }
    }

    Color Renderer::get_fill(Color color) {
        return fill_;
    }
    
    Color Renderer::get_borderfill() {
        return border_color_;
    }

    void Renderer::set_fill(Color color) {
        Vec2D logic_dim = display_->get_dimensions();
        draw_rect(nullptr, {logic_dim/2, logic_dim}, color, true);
        fill_ = color;
    }

    void Renderer::set_borderfill(Color color) {
        border_color_ = color;
    }

    void Renderer::draw_sprite(Sprite *dest, Sprite *source, Vec2D position,
                               RenderBlend mode) {
        AABB aabb = {position, source->get_dimensions()};
        if(!source->get_visible() || 
           source->get_alpha() <= 0) {
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
            display_->get_renderer(),
            source->get_base(),
            nullptr,
            &target_rect,
            source->get_angle(),
            nullptr,
            source->get_flip()
        );

        // Render actual source texture on its base
        set_render_target(source);
        SDL_SetRenderDrawColor(display_->get_renderer(), 0, 0, 0, 0);
        SDL_RenderClear(display_->get_renderer());
        SDL_RenderCopy(
            display_->get_renderer(), 
            source->get_texture(), 
            &source->get_source(), 
            nullptr
        );
        set_render_target(nullptr);
    }

    void Renderer::draw_point(Sprite *dest, Vec2D point, 
                              Color color, RenderBlend mode) {
        SDL_SetRenderDrawBlendMode(
            display_->get_renderer(),
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            display_->get_renderer(), 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        SDL_Point sdl_point = point.convert_to_point();
        SDL_RenderDrawPoint(
            display_->get_renderer(), 
            sdl_point.x,
            sdl_point.y
        );
        set_render_target(nullptr);
    }

    void Renderer::draw_line(Sprite *dest, Vec2D point1, Vec2D point2, 
                             Color color, RenderBlend mode) {
        SDL_SetRenderDrawBlendMode(
            display_->get_renderer(),
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            display_->get_renderer(), 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        SDL_Point sdl_p1 = point1.convert_to_point();
        SDL_Point sdl_p2 = point2.convert_to_point();
        SDL_RenderDrawLine(
            display_->get_renderer(), 
            sdl_p1.x,
            sdl_p1.y, 
            sdl_p2.x,
            sdl_p2.y
        );
        set_render_target(nullptr);
    }

    void Renderer::draw_rect(Sprite *dest, AABB box, 
                             Color color, bool fill, RenderBlend mode) {
        SDL_SetRenderDrawBlendMode(
            display_->get_renderer(),
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            display_->get_renderer(), 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        SDL_Rect rect = box.convert_to_rect();
        if(fill) {
            SDL_RenderFillRect(display_->get_renderer(), &rect);
        }
        else {
            SDL_RenderDrawRect(display_->get_renderer(), &rect);
        }
        set_render_target(nullptr);
    }

    void Renderer::draw_circle(Sprite *dest, Vec2D center, int radius, 
                               Color color, bool fill, RenderBlend mode) {
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
            display_->get_renderer(),
            static_cast<SDL_BlendMode>(mode)
        );
        SDL_SetRenderDrawColor(
            display_->get_renderer(), 
            color.r, color.g, color.b, color.a
        );

        set_render_target(dest);
        if(!fill) {
            SDL_RenderDrawPoints(
                display_->get_renderer(),
                &points[0],
                points.size()
            );
        }
        else {
            SDL_RenderDrawLines(
                display_->get_renderer(),
                &points[0],
                points.size()
            );
        }
        set_render_target(nullptr);
    }

    void Renderer::draw_polygon(Sprite *dest, Vec2D points[], int n, 
                                Color color, bool fill, RenderBlend mode) {
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

    void Renderer::refresh() {
        SDL_SetRenderDrawBlendMode(
            display_->get_renderer(), 
            SDL_BLENDMODE_NONE
        );
        SDL_SetRenderDrawColor(
            display_->get_renderer(), 
            border_color_.r, border_color_.g, border_color_.b, 0xFF
        );
        SDL_RenderPresent(display_->get_renderer());
        SDL_RenderClear(display_->get_renderer());
    }
}
