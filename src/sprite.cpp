#include "sprite.h"

namespace Dynamo {
	Sprite::Sprite(SDL_Texture *t, int frame_width, int frame_height) {
		texture = t;
		frame_w = frame_width;
		frame_h = frame_height;
		SDL_QueryTexture(texture, nullptr, nullptr, &texture_w, &texture_h);

		target = new SDL_Rect();

		accumulator = 0.0f;
		if(!frame_w && !frame_h) {
			max_frames = 1;
			source.push_back(nullptr);
		}
		else {
			int hor_frames = texture_w / frame_w;
			int ver_frames = texture_h / frame_h;
			max_frames = hor_frames * ver_frames;

			// Left to right, top to bottom
			for(int j = 0; j < ver_frames; j++) {
				for(int i = 0; i < hor_frames; i++) {
					SDL_Rect *frame_rect = new SDL_Rect();
					frame_rect->w = frame_w;
					frame_rect->h = frame_h;
					frame_rect->x = i*frame_w;
					frame_rect->y = j*frame_h;
					source.push_back(frame_rect);
				}
			}
		}

		current_frame = 0;

		finished = false;

		angle = 0.0f;
		hflip = false;
		vflip = false;
		visible = true;
	}

	Sprite::~Sprite() {
		delete target;
		for(auto &r : source) {
			delete r;
		}
		source.clear();
	}

	SDL_Texture *Sprite::get_texture() {
		return texture;
	}

	int Sprite::get_width() {
		return texture_w;
	}

	int Sprite::get_height() {
		return texture_h;
	}

	int Sprite::get_frame_height() {
		return frame_h;
	}

	int Sprite::get_frame_width() {
		return frame_w;
	}

	SDL_Rect *Sprite::get_source() {
		return source[current_frame];
	}

	SDL_Rect *Sprite::get_target() {
		return target;
	}

	bool Sprite::get_visible() {
		return visible;
	}

	float Sprite::get_angle() {
		return angle;
	}

	SDL_RendererFlip Sprite::get_flip() {
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		if(hflip && vflip) {
			flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		}
		else if(hflip) {
			flip = (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
		}
		else if(vflip) {
			flip = (SDL_RendererFlip)SDL_FLIP_VERTICAL;
		}
		return flip;
	}

	uint8_t Sprite::get_alpha() {
		uint8_t a;
		SDL_GetTextureAlphaMod(texture, &a);
		return a;
	}

	bool Sprite::get_finished() {
		return finished;
	}

	void Sprite::set_visible(bool new_visible) {
		visible = new_visible;
	}

	void Sprite::set_angle(float new_angle) {
		angle = new_angle;
	}

	void Sprite::set_flip(bool new_hflip, bool new_vflip) {
		hflip = new_hflip;
		vflip = new_vflip;
	}

	void Sprite::set_alpha(uint8_t a) {
		SDL_SetTextureAlphaMod(texture, a);
	}

	void Sprite::set_blend(SPRITE_BLEND mode) {
		SDL_SetTextureBlendMode(texture, static_cast<SDL_BlendMode>(mode));
	}

	void Sprite::set_target(int x, int y, int w, int h) {
		// One may pass the fields of a bounding box or something
		if(w < 1 || h < 1) {
			set_visible(false);
		}

		target->x = x;
		target->y = y;
		target->w = w;
		target->h = h;
	}

	void Sprite::animate(float dt, float fps, bool loop) {
		accumulator += dt;
		if(accumulator >= (1000.0/fps)) {
			current_frame++;
			accumulator = 0;
		}

		if(current_frame > max_frames-1) {
			if(loop) {
				current_frame = 0;
			}
			else {
				current_frame = max_frames-1;
				finished = true;
			}
		}
	}
}