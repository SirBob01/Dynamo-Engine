#ifndef DYNAMO_DISPLAY_H_
#define DYNAMO_DISPLAY_H_

#include <SDL2/SDL.h>

#include <string>

#include "sprite.h"
#include "color.h"

namespace Dynamo {
	class Display {
		int logic_w_;
		int logic_h_;

		int window_w_;
		int window_h_;

		SDL_Color border_color_;

		SDL_Window *window_;
		SDL_Renderer *renderer_;

	public:
		Display(int width, int height, std::string title);
		~Display();

		SDL_Window *get_window();
		SDL_Renderer *get_renderer();
		
		// Get the draw port dimensions
		int get_width();
		int get_height();

		// Get the physical GUI window dimentions
		int get_window_width();
		int get_window_height();

		// Set the GUI bar title
		void set_title(std::string title);

		// Set the fill of the draw port
		void set_fill(uint32_t rgb_color);

		// Set the fill of the stretched border
		void set_borderfill(uint32_t rgb_color);

		// Draw a single sprite
		void draw_sprite(Sprite *sprite);

		// Draw a single point
		void draw_point(int x, int y, uint32_t rgb_color);
		
		// Draw a line between two points (x1, y1) - (x2, y2)
		void draw_line(int x1, int y1, int x2, int y2, 
					   uint32_t rgb_color);
		
		// Draw a rectangle with upperleft corner at (x, y)
		void draw_rect(int x, int y, int w, int h, 
					   uint32_t rgb_color, bool fill);

		// Draw a circle of radius r with center (cx, cy)
		void draw_circle(int cx, int cy, int r, 
						 uint32_t rgb_color, bool fill);
		
		// Draw a polygon of n sides
		void draw_polygon(int points[][2], int n, uint32_t rgb_color);

		// Refresh the display after each game-loop iteration
		void refresh();
	};
}

#endif