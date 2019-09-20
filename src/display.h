#ifndef GAME_DISPLAY
#define GAME_DISPLAY

#include <SDL2/SDL.h>
#include <string>

#include "sprite.h"
#include "color.h"

class GameDisplay {
	int logic_width;
	int logic_height;

	int window_w;
	int window_h;

	SDL_Color border_color;

	SDL_Window *window;
	SDL_Renderer *renderer;

public:
	GameDisplay(int width, int height, std::string title);
	~GameDisplay();

	int get_width();
	int get_height();

	int get_window_width();
	int get_window_height();

	SDL_Window *get_window();
	SDL_Renderer *get_renderer();

	void set_title(std::string title);
	void set_fill(uint32_t rgb_color);
	void set_borderfill(uint32_t rgb_color);

	void draw_sprite(GameSprite *sprite);

	// Drawing primitives
	void draw_point(int x, int y, uint32_t rgb_color);
	void draw_line(int x1, int y1, int x2, int y2, uint32_t rgb_color);
	void draw_rect(int x, int y, int w, int h, uint32_t rgb_color, bool fill);
	void draw_circle(int cx, int cy, int r, uint32_t rgb_color, bool fill);
	void draw_polygon(int points[][2], int n, uint32_t rgb_color);

	void refresh();
};

#endif