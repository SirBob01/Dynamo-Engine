#ifndef DYNAMO_RENDERER_H_
#define DYNAMO_RENDERER_H_

#include <SDL2/SDL.h>

#include "display.h"
#include "../util/aabb.h"
#include "../util/vector.h"
#include "../util/color.h"
#include "../sprite/sprite.h"

namespace Dynamo {
    enum class RenderBlend {
        None = SDL_BLENDMODE_NONE,
        Blend = SDL_BLENDMODE_BLEND,
        Add = SDL_BLENDMODE_ADD,
        Mod = SDL_BLENDMODE_MOD,
    };

    class Renderer {
        Display *display_;
        Color fill_;
        Color border_color_;

        void set_render_target(Sprite *dest);
    
    public:
        Renderer(Display *display, bool vsync=false);

        // Get the current base color of the renderer
        Color get_fill(Color color);

        // Get the color of the stretched border 
        Color get_borderfill();

        // Set the fill of the draw port
        void set_fill(Color color);

        // Set the fill of the stretched border
        void set_borderfill(Color color);

        // Draw a sprite
        // TODO: Fix weird blending between sprites
        // Hint: Difference between alpha channel and alpha mod?
        // Answer: Yes, and I don't fucking know why
        void draw_sprite(Sprite *dest, Sprite *source, Vec2D position, 
                         RenderBlend mode=RenderBlend::Blend);

        // Draw a single point
        void draw_point(Sprite *dest, Vec2D point, Color color, 
                        RenderBlend mode=RenderBlend::Blend);
        
        // Draw a line between two points
        void draw_line(Sprite *dest, Vec2D point1, Vec2D point2, Color color, 
                       RenderBlend mode=RenderBlend::Blend);
        
        // Draw a rectangle
        void draw_rect(Sprite *dest, AABB box, Color color, bool fill, 
                       RenderBlend mode=RenderBlend::Blend);

        // Draw a circle
        void draw_circle(Sprite *dest, Vec2D center, int radius, 
                         Color color, bool fill, 
                         RenderBlend mode=RenderBlend::Blend);
        
        // Draw a polygon of n sides
        void draw_polygon(Sprite *dest, Vec2D points[], int n, 
                          Color color, bool fill, 
                          RenderBlend mode=RenderBlend::Blend);

        // Refresh the renderer after each game-loop iteration
        void refresh();
    };
}

#endif