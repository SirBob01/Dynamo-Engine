#include "sprite.h"

namespace Dynamo {
    Sprite::Sprite(Texture texture, Vec2D frame_dimensions) {
        texture_ = texture.texture;
        SDL_QueryTexture(
            texture_, 
            nullptr, nullptr, 
            &texture_w_, &texture_h_
        );
        texture_dimensions_ = {
            static_cast<float>(texture_w_),
            static_cast<float>(texture_h_)
        };

        // Calculate frame_dimensions
        if(frame_dimensions.x && frame_dimensions.x <= texture_dimensions_.x) {
            frame_dimensions_.x = frame_dimensions.x;
        }
        else {
            frame_dimensions_.x = texture_dimensions_.x;
        }

        if(frame_dimensions.y && frame_dimensions.y <= texture_dimensions_.y) {
            frame_dimensions_.y = frame_dimensions.y;
        }
        else {
            frame_dimensions_.y = texture_dimensions_.y;
        }

        // Generate base texture
        SDL_Point sdl_frame = frame_dimensions_.convert_to_point();
        base_ = SDL_CreateTexture(
            texture.renderer, 
            SDL_PIXELFORMAT_RGBA8888, 
            SDL_TEXTUREACCESS_TARGET, 
            sdl_frame.x, sdl_frame.y
        );
        if(!base_) {
            throw SDLError(SDL_GetError());
        }

        target_dimensions_ = frame_dimensions_;

        int hor_frames = texture_dimensions_.x / frame_dimensions_.x;
        int ver_frames = texture_dimensions_.y / frame_dimensions_.y;
        max_frames_ = hor_frames * ver_frames;

        // Left to right, top to bottom
        for(int j = 0; j < ver_frames; j++) {
            for(int i = 0; i < hor_frames; i++) {
                SDL_Rect *frame_rect = new SDL_Rect();

                frame_rect->w = frame_dimensions_.x;
                frame_rect->h = frame_dimensions_.y;
                frame_rect->x = i*frame_dimensions_.x;
                frame_rect->y = j*frame_dimensions_.y;
                
                source_.push_back(frame_rect);
            }
        }

        scale_ = 1.0;
        
        accumulator_ = 0.0f;
        current_frame_ = 0;

        finished_ = false;

        angle_ = 0.0f;
        hflip_ = false;
        vflip_ = false;
        visible_ = true;
    }

    Sprite::~Sprite() {
        SDL_DestroyTexture(base_);
        for(auto &r : source_) {
            delete r;
        }
        source_.clear();
    }

    SDL_Texture *Sprite::get_texture() {
        return texture_;
    }

    SDL_Texture *Sprite::get_base() {
        return base_;
    }

    Vec2D Sprite::get_texture_dimensions() {
        return texture_dimensions_;
    }

    Vec2D Sprite::get_frame_dimensions() {
        return frame_dimensions_;
    }

    Vec2D Sprite::get_dimensions() {
        return target_dimensions_ * scale_;
    }

    SDL_Rect *Sprite::get_source() {
        return source_[current_frame_];
    }

    int Sprite::get_num_frames() {
        return max_frames_;
    }

    int Sprite::get_frame_index() {
        return current_frame_;
    }

    bool Sprite::get_visible() {
        return visible_;
    }

    float Sprite::get_angle() {
        return angle_;
    }

    SDL_RendererFlip Sprite::get_flip() {
        int flip = SDL_FLIP_NONE;
        if(hflip_ && vflip_) {
            flip = (SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
        }
        else if(hflip_) {
            flip = SDL_FLIP_HORIZONTAL;
        }
        else if(vflip_) {
            flip = SDL_FLIP_VERTICAL;
        }
        
        return static_cast<SDL_RendererFlip>(flip);
    }

    uint8_t Sprite::get_alpha() {
        uint8_t alpha;
        SDL_GetTextureAlphaMod(base_, &alpha);
        return alpha;
    }

    bool Sprite::get_finished() {
        return finished_;
    }

    void Sprite::set_visible(bool visible) {
        visible_ = visible;
    }

    void Sprite::set_angle(float angle) {
        angle_ = angle;
    }

    void Sprite::set_flip(bool hflip, bool vflip) {
        hflip_ = hflip;
        vflip_ = vflip;
    }

    void Sprite::set_alpha(uint8_t alpha) {
        SDL_SetTextureAlphaMod(base_, alpha);
    }

    void Sprite::set_dimensions(Vec2D dimensions) {
        target_dimensions_ = dimensions;
    }

    void Sprite::set_scale(float scale) {
        scale_ = scale;
    }

    void Sprite::set_frame(int index) {
        current_frame_ = index;
        if(current_frame_ < 0 || current_frame_ >= max_frames_) {
            current_frame_ %= max_frames_;
        }
    }

    void Sprite::shift_frame(int direction) {
        if(direction < 0) {
            current_frame_--;
        }
        else {
            current_frame_++;
        }

        if(current_frame_ < 0 || current_frame_ >= max_frames_) {
            current_frame_ %= max_frames_;
        }
    }

    void Sprite::animate(unsigned dt, unsigned fps, bool loop) {
        accumulator_ += dt;
        if(accumulator_ >= (1000.0/fps)) {
            current_frame_++;
            accumulator_ = 0;
        }

        if(current_frame_ > max_frames_-1) {
            if(loop) {
                current_frame_ = 0;
            }
            else {
                current_frame_ = max_frames_-1;
                finished_ = true;
            }
        }
    }
}