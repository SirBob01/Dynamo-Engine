#ifndef DYNAMO_SPRITE_H_
#define DYNAMO_SPRITE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>
#include <iostream>

namespace Dynamo {
	typedef enum {
		SPRITE_BLEND_NONE = SDL_BLENDMODE_NONE,
		SPRITE_BLEND_BLEND = SDL_BLENDMODE_BLEND,
		SPRITE_BLEND_ADD = SDL_BLENDMODE_ADD,
		SPRITE_BLEND_MOD = SDL_BLENDMODE_MOD,
	} SPRITE_BLEND;

	class Sprite {
		SDL_Texture *texture;
		std::vector<SDL_Rect *> source;

		SDL_Rect *target;

		int texture_w;
		int texture_h;
		int frame_w;
		int frame_h;
		
		float accumulator;
		int max_frames;
		int current_frame;

		bool finished;
		
		// Draw modifiers
		float angle;
		bool hflip;
		bool vflip;
		bool visible;

	public:
		Sprite(SDL_Texture *t, int frame_width, int frame_height);
		~Sprite();

		// Get spritesheet texture (all frames)
		SDL_Texture *get_texture();

		// Get texture dimensions
		int get_width();
		int get_height();

		// Get individual frame dimensions
		int get_frame_width();
		int get_frame_height();

		// Get the rectangle of an individual animation frame
		SDL_Rect *get_source();

		// Get the draw target rectangle
		SDL_Rect *get_target();

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
		void set_visible(bool new_visible);

		// Set angular rotation
		void set_angle(float new_angle);

		// Set horizontal and vertical flip
		void set_flip(bool new_hflip, bool new_vflip);

		// Set texture alpha channel
		void set_alpha(uint8_t a);

		// Set texture blend mode
		void set_blend(SPRITE_BLEND mode);

		// Set the draw target position and dimensions
		void set_target(int x, int y, int w, int h);

		// Animate the frames of the sprite
		void animate(float dt, float fps, bool loop);
	};
}

#endif