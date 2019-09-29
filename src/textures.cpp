#include "textures.h"

Dynamo::Textures::Textures(SDL_Renderer *r) {
	renderer = r;
	TTF_Init();
}

Dynamo::Textures::~Textures() {
	clear_all();
	TTF_Quit();
}

void Dynamo::Textures::load_surface(std::string id, int width, int height) {
	if(texture_map.count(id)) {
		SDL_DestroyTexture(texture_map[id]);
	}

	SDL_Surface *surface;
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		surface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	}
	else {
		surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	}
	
	texture_map[id] = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Dynamo::Textures::load_image(std::string id, std::string filename) {
	if(texture_map.count(id)) {
		SDL_DestroyTexture(texture_map[id]);
	}

	texture_map[id] = IMG_LoadTexture(renderer, filename.c_str());
}

void Dynamo::Textures::load_text(std::string id, std::string text, std::string font_id, uint32_t rgb_color) {
	if(texture_map.count(id)) {
		SDL_DestroyTexture(texture_map[id]);
	}

	SDL_Surface *surf = TTF_RenderText_Solid(fonts[font_id], text.c_str(), hex_to_rgb(rgb_color));
	texture_map[id] = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
}

void Dynamo::Textures::load_font(std::string font_id, std::string filename, int size) {
	if(fonts.count(font_id)) {
		TTF_CloseFont(fonts[font_id]);
	}

	fonts[font_id] = TTF_OpenFont(filename.c_str(), size);
}

SDL_Texture *Dynamo::Textures::get_texture(std::string id) {
	return texture_map[id];
}

void Dynamo::Textures::clear_all() {
	for(auto &item : texture_map) {
		SDL_DestroyTexture(item.second);
	}
	for(auto &font : fonts) {
		TTF_CloseFont(font.second);
	}
	texture_map.clear();
	fonts.clear();
}