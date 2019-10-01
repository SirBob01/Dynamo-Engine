#include "inputs.h"
#include <iostream>
namespace Dynamo {
    Inputs::Inputs() {
        text_input_ = "";
        quit_ = false;
        std::memset(pressed_, false, (INPUT_LEN + 1) * sizeof(bool));
        std::memset(released_, false, (INPUT_LEN + 1) * sizeof(bool));
    }

    void Inputs::poll() {
        for(auto &key : pressed_change_) {
            pressed_[key] = false;
        }
        for(auto &key : released_change_) {
            released_[key] = false;
        }
        pressed_change_.clear();
        released_change_.clear();

        SDL_GetMouseState(&mouse_x_, &mouse_y_);

        while(SDL_PollEvent(&event_)) {
            if(event_.type == SDL_QUIT) {
                quit_ = true;
            }
            
            if(event_.type == SDL_MOUSEBUTTONDOWN) {
                uint8_t button = event_.button.button;
                for(int i = INPUT_MOUSELEFT; i <= INPUT_MOUSERIGHT; i++) {
                    pressed_[i] = (button - 1 == i - INPUT_MOUSELEFT);
                    if(pressed_[i]) {
                        pressed_change_.push_back(i);
                    }
                }
            }
            
            if(event_.type == SDL_MOUSEBUTTONUP) {
                uint8_t button = event_.button.button;
                for(int i = INPUT_MOUSELEFT; i <= INPUT_MOUSERIGHT; i++) {
                    released_[i] = (button - 1 == i - INPUT_MOUSELEFT);
                    if(released_[i]) {
                        released_change_.push_back(i);
                    }
                }
            }

            if(event_.type == SDL_KEYDOWN && event_.key.repeat == 0) {
                int scancode = event_.key.keysym.scancode;
                pressed_[scancode] = true;
                pressed_change_.push_back(scancode);

                // Special backspace event
                if(text_input_.size()) {
                    if(event_.key.keysym.sym == SDLK_BACKSPACE) {
                        text_input_.erase(text_input_.size()-1);
                    }
                }
            }

            if(event_.type == SDL_KEYUP) {
                int scancode = event_.key.keysym.scancode;
                released_[scancode] = true;
                released_change_.push_back(scancode);
            }
            
            if(event_.type == SDL_TEXTINPUT) {
                text_input_ += event_.text.text;
            }
        }
    }

    std::string Inputs::get_text_input() {
        return text_input_;
    }

    void Inputs::reset_text_input() {
        text_input_ = "";
    }

    void Inputs::bind(std::string command, INPUT_CODE input) {
        binds_[command] = input;
    }

    INPUT_CODE Inputs::get_bind(std::string command) {
        return binds_[command];
    }

    bool Inputs::get_pressed(std::string command) {
        return get_pressed_raw(binds_[command]);
    }

    bool Inputs::get_released(std::string command) {
        return get_released_raw(binds_[command]);
    }

    bool Inputs::get_pressed_raw(INPUT_CODE input) {
        return pressed_[input];
    }

    bool Inputs::get_released_raw(INPUT_CODE input) {
        return released_[input];
    }

    std::string Inputs::get_name(INPUT_CODE input) {
        std::string s = " ";
        if(input < INPUT_MOUSELEFT) {
            s = SDL_GetScancodeName(static_cast<SDL_Scancode>(input));
        }
        else {
            switch(input) {
                case INPUT_MOUSELEFT:
                    s = "Left Mouse";
                    break;
                case INPUT_MOUSEMIDDLE:
                    s = "Middle Mouse";
                    break;
                case INPUT_MOUSERIGHT:
                    s = "Right Mouse";
                    break;
                default:
                    break;
            }
        }
        return s;
    }

    Vec2D Inputs::get_mouse_pos() {
        return {
            static_cast<float>(mouse_x_),
            static_cast<float>(mouse_y_),
        };
    }

    bool Inputs::get_quit() {
        return quit_;
    }
}