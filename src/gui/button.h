#ifndef DYNAMO_GUI_BUTTON_H_
#define DYNAMO_GUI_BUTTON_H_

#include "widget.h"
#include "../sprite.h"
#include "../textures.h"

namespace Dynamo::GUI {
    class Button : public Widget {
        Sprite *sprites_[2];
        int sprite_count_;

    public:
        Button(Texture base, Texture hover, Vec2D frame_size);
        ~Button();

        // Get current sprite
        Sprite *get_sprite();
    };
}

#endif