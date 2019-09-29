#include "display.h"

namespace Dynamo {
	Display::Display(int width, int height, std::string title) {
		logic_width = width;
		logic_height = height;

		window = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			SDL_WINDOW_RESIZABLE
		);

		border_color = {0, 0, 0};

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		SDL_RenderSetLogicalSize(renderer, width, height);
	}

	Display::~Display() {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	int Display::get_width() {
		return logic_width;
	}

	int Display::get_height() {
		return logic_height;
	}

	int Display::get_window_width() {
		SDL_GetWindowSize(window, &window_w, nullptr);
		return window_w;
	}

	int Display::get_window_height() {
		SDL_GetWindowSize(window, nullptr, &window_h);
		return window_h;
	}

	SDL_Window *Display::get_window() {
		return window;
	}

	SDL_Renderer *Display::get_renderer() {
		return renderer;
	}

	void Display::set_title(std::string title) {
		SDL_SetWindowTitle(window, title.c_str());
	}

	void Display::set_fill(uint32_t rgb_color) {
		draw_rect(0, 0, logic_width, logic_height, rgb_color, true);
	}

	void Display::set_borderfill(uint32_t rgb_color) {
		border_color = hex_to_rgb(rgb_color);
	}

	void Display::draw_sprite(Sprite *sprite) {
		if(sprite->get_visible() && sprite->get_alpha()) {
			SDL_RenderCopyEx(
				renderer,
				sprite->get_texture(),
				sprite->get_source(),
				sprite->get_target(),
				sprite->get_angle(),
				nullptr,
				sprite->get_flip()
			);
		}
	}

	void Display::draw_point(int x, int y, uint32_t rgb_color) {
		SDL_Color color = hex_to_rgb(rgb_color);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
		SDL_RenderDrawPoint(renderer, x, y);
	}

	void Display::draw_line(int x1, int y1, int x2, int y2, uint32_t rgb_color) {
		SDL_Color color = hex_to_rgb(rgb_color);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	void Display::draw_rect(int x, int y, int w, int h, uint32_t rgb_color, bool fill) {
		SDL_Color color = hex_to_rgb(rgb_color);
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0xFF);
		SDL_Rect rect = {x, y, w, h};
		if(fill) {
			SDL_RenderFillRect(renderer, &rect);
		}
		else {
			SDL_RenderDrawRect(renderer, &rect);
		}
	}

	void Display::draw_circle(int cx, int cy, int r, uint32_t rgb_color, bool fill) {
		// Midpoint algorithm
		int x = r;
		int y = 0;
		int p = 1 - r;
		while(x >= y) {
			if(!fill) {
				draw_point(cx+x, cy+y, rgb_color);
				draw_point(cx+x, cy-y, rgb_color);
				draw_point(cx-x, cy+y, rgb_color);
				draw_point(cx-x, cy-y, rgb_color);
				draw_point(cx+y, cy+x, rgb_color);
				draw_point(cx+y, cy-x, rgb_color);
				draw_point(cx-y, cy+x, rgb_color);
				draw_point(cx-y, cy-x, rgb_color);
			}
			else {
				draw_line(cx+x, cy+y, cx-x, cy+y, rgb_color);
				draw_line(cx+y, cy+x, cx-y, cy+x, rgb_color);
				draw_line(cx+x, cy-y, cx-x, cy-y, rgb_color);
				draw_line(cx+y, cy-x, cx-y, cy-x, rgb_color);
			}
			if(p <= 0) {
				p += 2*y + 1;
			}
			else {
				x--;
				p += 2*y - 2*x + 1;
			}
			y++;
		}
	}

	void Display::refresh() {
		SDL_SetRenderDrawColor(renderer, border_color.r, border_color.g, border_color.b, 0xFF);
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
}