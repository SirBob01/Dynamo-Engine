#ifndef DYNAMO_GUI_BUTTON_H_
#define DYNAMO_GUI_BUTTON_H_

#include "widget.h"
#include "../sprite/sprite.h"
#include "../core/textures.h"

namespace Dynamo::GUI {
    // Simplest type of GUI widget
    class Button : public Widget {
        Sprite *sprites_[2];
        int sprite_count_;

    public:
        Button(SDL_Texture *base, SDL_Texture *hover, Vec2D frame_size);
        ~Button();

        // Get current sprite
        Sprite *get_sprite();
    };
}

#endif