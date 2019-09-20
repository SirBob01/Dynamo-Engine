#ifndef GAME_TEXTURES
#define GAME_TEXTURES

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <string>

#include "color.h"

class GameTextures {
	SDL_Renderer *renderer;
	
	std::unordered_map<std::string, TTF_Font *> fonts;
	std::unordered_map<std::string, SDL_Texture *> texture_map;

public:
	GameTextures(SDL_Renderer *r);
	~GameTextures();

	// Pre-load resources (on Scene initialization)
	// If texture exists in map, it is overwritten by new texture
	// Prevents memory leaks (don't trust user)
	void load_surface(std::string id, int width, int height);
	void load_image(std::string id, std::string filename);
	void load_text(std::string id, std::string text, std::string font_id, uint32_t rgb_color);
	void load_font(std::string font_id, std::string filename, int size);

	SDL_Texture *get_texture(std::string id);

	void clear_all();
};

#endif