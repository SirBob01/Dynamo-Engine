#ifndef GAME_SPRITE
#define GAME_SPRITE

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

typedef enum {
	GAME_BLEND_NONE = SDL_BLENDMODE_NONE,
	GAME_BLEND_BLEND = SDL_BLENDMODE_BLEND,
	GAME_BLEND_ADD = SDL_BLENDMODE_ADD,
	GAME_BLEND_MOD = SDL_BLENDMODE_MOD,
} GAME_BLEND;

class GameSprite {
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
	GameSprite(SDL_Texture *t, int frame_width, int frame_height);
	~GameSprite();

	// Getters
	SDL_Texture *get_texture();

	int get_width();
	int get_height();

	int get_frame_width();
	int get_frame_height();

	SDL_Rect *get_source();
	SDL_Rect *get_target();

	bool get_visible();
	float get_angle();
	SDL_RendererFlip get_flip();
	uint8_t get_alpha();
	bool get_finished();

	// Setters
	void set_visible(bool new_visible);
	void set_angle(float new_angle);
	void set_flip(bool new_hflip, bool new_vflip);

	void set_alpha(uint8_t a);
	void set_blend(GAME_BLEND mode);
	
	void set_finished(bool new_finished);

	void set_target(int x, int y, int w, int h);

	void animate(float dt, float fps, bool loop);
	void draw();
};

#endif