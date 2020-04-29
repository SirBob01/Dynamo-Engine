#ifndef DYNAMO_RENDERER_H_
#define DYNAMO_RENDERER_H_

#include <SDL2/SDL.h>

#include "../util/color.h"
#include "../util/aabb.h"
#include "../util/vector.h"
#include "../sprite/sprite.h"

namespace Dynamo {
    typedef enum {
        RENDER_BLEND_NONE = SDL_BLENDMODE_NONE,
        RENDER_BLEND_BLEND = SDL_BLENDMODE_BLEND,
        RENDER_BLEND_ADD = SDL_BLENDMODE_ADD,
        RENDER_BLEND_MOD = SDL_BLENDMODE_MOD,
    } RENDER_BLEND;

    class Renderer {
        SDL_Renderer *renderer_;

        AABB logic_bounds_;

        Color fill_;
        Color border_color_;

        void set_render_target(Sprite *dest);
    
    public:
        Renderer(SDL_Window *window);
        ~Renderer();

        //Get the pointer to the SDL renderer
        SDL_Renderer *get_renderer();

        // Get the logical dimensions of the renderer
        Vec2D get_dimensions();

        // Get the current base color of the renderer
        Color get_fill(Color color);

        // Get the color of the stretched border 
        Color get_borderfill();

        // Set the logical dimensions of the renderer
        void set_dimensions(int x, int y);

        // Set the fill of the draw port
        void set_fill(Color color);

        // Set the fill of the stretched border
        void set_borderfill(Color color);

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
        void draw_polygon(Sprite *dest, Vec2D points[], int n, 
                          Color color, bool fill, 
                          RENDER_BLEND mode=RENDER_BLEND_BLEND);

        // Refresh the renderer after each game-loop iteration
        void refresh();
    };
}

#endif