#ifndef DYNAMO_SPRITE_H_
#define DYNAMO_SPRITE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>

#include "../core/textures.h"
#include "../util/aabb.h"
#include "../util/vec2d.h"

namespace Dynamo {
    typedef enum {
        SPRITE_BLEND_NONE = SDL_BLENDMODE_NONE,
        SPRITE_BLEND_BLEND = SDL_BLENDMODE_BLEND,
        SPRITE_BLEND_ADD = SDL_BLENDMODE_ADD,
        SPRITE_BLEND_MOD = SDL_BLENDMODE_MOD,
    } SPRITE_BLEND;

    class Sprite {
        SDL_Texture *texture_;
        int texture_w_;
        int texture_h_;

        Vec2D texture_dimensions_;
        Vec2D frame_dimensions_;
        Vec2D target_dimensions_;

        std::vector<SDL_Rect *> source_;
        
        float accumulator_;
        int max_frames_;
        int current_frame_;

        bool finished_;
        
        // Draw modifiers
        float angle_;
        bool hflip_;
        bool vflip_;
        bool visible_;

    public:
        // TODO: Create deep copy of SDL_Texture pointer
        Sprite(SDL_Texture *texture, Vec2D frame_dimensions={0, 0});
        ~Sprite();

        // Get spritesheet texture (all frames)
        SDL_Texture *get_texture();

        // Get the whole texture's dimensions
        Vec2D get_texture_dimensions();

        // Get an individual frame's dimensions
        Vec2D get_frame_dimensions();

        // Get the target dimensions
        Vec2D get_dimensions();

        // Get the rectangle of an individual animation frame
        SDL_Rect *get_source();

        // Get number of animation frames
        int get_num_frames();

        // Get current frame index
        int get_frame_index();

        // Check whether or not to draw the sprite
        bool get_visible();

        // Get angular rotation
        float get_angle();

        // Get texture flip state for rendering
        SDL_RendererFlip get_flip();

        // Get texture alpha channel
        uint8_t get_alpha();

        // Check if the current sprite animation is finished
        bool get_finished();

        // Set whether or not to draw the sprite
        void set_visible(bool visible);

        // Set angular rotation
        void set_angle(float angle);

        // Set horizontal and vertical flip
        void set_flip(bool hflip, bool vflip);

        // Set texture alpha channel
        void set_alpha(uint8_t alpha);

        // Set texture blend mode
        void set_blend(SPRITE_BLEND mode);

        // Set the draw target dimension
        void set_dimensions(Vec2D dimensions);

        // Set the current animation frame
        void set_frame(int index);

        // Shift animation frame by 1 unit
        void shift_frame(int direction);

        // Animate the frames of the sprite
        void animate(unsigned dt, unsigned fps, bool loop);
    };
}

#endif