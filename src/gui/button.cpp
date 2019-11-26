#include "button.h"

namespace Dynamo::GUI {
    Button::Button(Texture base, Texture hover, Vec2D frame_size) : Widget() {
        sprites_[0] = new Sprite(base, frame_size);
        sprites_[1] = new Sprite(hover, frame_size);
        sprite_count_ = 2;
    }

    Button::~Button() {
        for(int i = 0; i < sprite_count_; i++) {
            delete sprites_[i];
        }
    }

    Sprite *Button::get_sprite() {
        Sprite *target = sprites_[state_%sprite_count_];
        target->set_target(bounds_);
        return target;
    }
}