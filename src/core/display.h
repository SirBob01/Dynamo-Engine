#ifndef DYNAMO_DISPLAY_H_
#define DYNAMO_DISPLAY_H_

#include <SDL2/SDL.h>

#include <string>

#include "../sprite/sprite.h"
#include "../util/color.h"
#include "../util/aabb.h"
#include "../util/vec2d.h"

namespace Dynamo {
    typedef enum {
        RENDER_BLEND_NONE = SDL_BLENDMODE_NONE,
        RENDER_BLEND_BLEND = SDL_BLENDMODE_BLEND,
        RENDER_BLEND_ADD = SDL_BLENDMODE_ADD,
        RENDER_BLEND_MOD = SDL_BLENDMODE_MOD,
    } RENDER_BLEND;

    class Display {
        int window_x_;
        int window_y_;

        Vec2D logic_dim_;

        Color border_color_;

        SDL_Window *window_;
        SDL_Renderer *renderer_;

        void set_render_target(Sprite *dest);

    public:
        Display(int width, int height, std::string title);
        ~Display();

        SDL_Window *get_window();
        SDL_Renderer *get_renderer();

        // Get the draw port dimensions
        Vec2D get_dimensions();

        // Get the physical GUI window dimentions
        Vec2D get_window_dimensions();

        // Set the GUI bar title
        void set_title(std::string title);

        // Set the fill of the draw port
        void set_fill(Color color);

        // Set the fill of the stretched border
        void set_borderfill(Color color);

        // Get the title of the window
        std::string get_title();

        // Get the color of the draw port
        Color get_fill();

        // Get the color of the stretched border 
        Color get_borderfill();

        // Draw a sprite
        // TODO: Fix weird blending between sprites
        // Hint: Difference between alpha channel and alpha mod?
        // Answer: Yes, and I don't fucking know why
        void draw_sprite(Sprite *dest, Sprite *source, Vec2D position, 
                         RENDER_BLEND mode=RENDER_BLEND_BLEND);

        // Draw a single point
        void draw_point(Sprite *dest, Vec2D point, Color color, 
                        RENDER_BLEND mode=RENDER_BLEND_BLEND);
        
        // Draw a line between two points
        void draw_line(Sprite *dest, Vec2D point1, Vec2D point2, Color color, 
                       RENDER_BLEND mode=RENDER_BLEND_BLEND);
        
        // Draw a rectangle
        void draw_rect(Sprite *dest, AABB box, Color color, bool fill, 
                       RENDER_BLEND mode=RENDER_BLEND_BLEND);

        // Draw a circle
        void draw_circle(Sprite *dest, Vec2D center, int radius, 
                         Color color, bool fill, 
                         RENDER_BLEND mode=RENDER_BLEND_BLEND);
        
        // Draw a polygon of n sides
        // TODO: Implement filled polygons
        void draw_polygon(Sprite *dest, Vec2D points[], int n, Color color, 
                          RENDER_BLEND mode=RENDER_BLEND_BLEND);

        // Refresh the display after each game-loop iteration
        void refresh();
    };
}

#endif