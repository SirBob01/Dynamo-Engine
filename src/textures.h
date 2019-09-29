#ifndef DYNAMO_TEXTURES_H_
#define DYNAMO_TEXTURES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <unordered_map>
#include <string>

#include "color.h"

namespace Dynamo {
	class Textures {
		SDL_Renderer *renderer_;
		
		std::unordered_map<std::string, TTF_Font *> fonts_;
		std::unordered_map<std::string, SDL_Texture *> texture_map_;

	public:
		Textures(SDL_Renderer *renderer);
		~Textures();

		// Pre-load resources on Scene initialization
		// If texture exists, it is overwritten to prevent memory leaks
		void load_surface(std::string id, int width, int height);
		void load_image(std::string id, std::string filename);
		void load_text(std::string id, std::string text, std::string font_id, Color color);
		void load_font(std::string font_id, std::string filename, int size);

		// Get a texture from an id key
		SDL_Texture *get_texture(std::string id);

		// Free all loaded textures from memory
		void clear_all();
	};	
}

#endif