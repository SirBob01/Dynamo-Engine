#ifndef DYNAMO_DISPLAY_H_
#define DYNAMO_DISPLAY_H_

#include <SDL2/SDL.h>

#include <string>

#include "../sprite/sprite.h"
#include "../util/color.h"
#include "../util/aabb.h"
#include "../util/vec2d.h"

namespace Dynamo {
    class Display {
        int window_x_;
        int window_y_;

        Vec2D logic_dim_;

        Color border_color_;

        SDL_Window *window_;
        SDL_Renderer *renderer_;

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

        // Draw a single sprite
        void draw_sprite(Sprite *sprite, Vec2D position, bool center=true);

        // Draw a single point
        void draw_point(Vec2D point, Color color);
        
        // Draw a line between two points
        void draw_line(Vec2D point1, Vec2D point2, 
                          Color color);
        
        // Draw a rectangle from the upperleft corner point
        void draw_rect(AABB box, Color color, bool fill, bool center=true);

        // Draw a circle
        void draw_circle(Vec2D center, int radius, 
                             Color color, bool fill);
        
        // Draw a polygon of n sides
        void draw_polygon(Vec2D points[], int n, Color color);

        // Refresh the display after each game-loop iteration
        void refresh();
    };
}

#endif