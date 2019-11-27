#include "sprite.h"

namespace Dynamo {
    Sprite::Sprite(SDL_Texture *texture, Vec2D frame_dimensions) {
        texture_ = texture;
        SDL_QueryTexture(
            texture_, 
            nullptr, nullptr, 
            &texture_w_, &texture_h_
        );

        // Default blend mode
        SDL_SetTextureBlendMode(
            texture_, 
            SDL_BLENDMODE_BLEND
        );

        if(frame_dimensions.x) {
            frame_w_ = frame_dimensions.x;    
        }
        else {
            frame_w_ = texture_w_;
        }

        if(frame_dimensions.y) {
            frame_h_ = frame_dimensions.y;
        }
        else {
            frame_h_ = texture_h_;
        }

        target_ = new SDL_Rect();
        int hor_frames = texture_w_ / frame_w_;
        int ver_frames = texture_h_ / frame_h_;
        max_frames_ = hor_frames * ver_frames;

        // Left to right, top to bottom
        for(int j = 0; j < ver_frames; j++) {
            for(int i = 0; i < hor_frames; i++) {
                SDL_Rect *frame_rect = new SDL_Rect();

                frame_rect->w = frame_w_;
                frame_rect->h = frame_h_;
                frame_rect->x = i*frame_w_;
                frame_rect->y = j*frame_h_;
                
                source_.push_back(frame_rect);
            }
        }
        
        accumulator_ = 0.0f;
        current_frame_ = 0;

        finished_ = false;

        angle_ = 0.0f;
        hflip_ = false;
        vflip_ = false;
        visible_ = true;
    }

    Sprite::~Sprite() {
        SDL_DestroyTexture(texture_);

        delete target_;
        for(auto &r : source_) {
            delete r;
        }
        source_.clear();
    }

    SDL_Texture *Sprite::get_texture() {
        return texture_;
    }

    Vec2D Sprite::get_texture_dimensions() {
        return {
            static_cast<float>(texture_w_),
            static_cast<float>(texture_h_),
        };
    }

    Vec2D Sprite::get_dimensions() {
        return {
            static_cast<float>(frame_w_),
            static_cast<float>(frame_h_),
        };
    }

    SDL_Rect *Sprite::get_source() {
        return source_[current_frame_];
    }

    SDL_Rect *Sprite::get_target() {
        return target_;
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
        
        return (SDL_RendererFlip)flip;
    }

    uint8_t Sprite::get_alpha() {
        uint8_t alpha;
        SDL_GetTextureAlphaMod(texture_, &alpha);
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
        SDL_SetTextureAlphaMod(texture_, alpha);
    }

    void Sprite::set_blend(SPRITE_BLEND mode) {
        SDL_SetTextureBlendMode(
            texture_, 
            static_cast<SDL_BlendMode>(mode)
        );
    }

    void Sprite::set_target(AABB box, bool center) {
        Vec2D top_left;
        if(center) {
            top_left = box.get_min();
        }
        else {
            top_left = box.center;
        }
        target_->x = static_cast<int>(top_left.x);
        target_->y = static_cast<int>(top_left.y);
        target_->w = static_cast<int>(box.dim.x);
        target_->h = static_cast<int>(box.dim.y);
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

    void Sprite::animate(int dt, int fps, bool loop) {
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