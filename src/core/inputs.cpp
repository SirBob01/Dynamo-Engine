#include "inputs.h"

namespace Dynamo {
    Inputs::Inputs() {
        text_input_ = "";
        quit_ = false;

        scaled_mouse_ = {0, 0};

        std::memset(pressed_, false, (INPUT_LEN + 1) * sizeof(bool));
        std::memset(released_, false, (INPUT_LEN + 1) * sizeof(bool));
        std::memset(state_, false, (INPUT_LEN + 1) * sizeof(bool));
    }

    void Inputs::poll() {
        reset_states();
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
                    if(button - 1 == i - INPUT_MOUSELEFT) {
                        state_[i] = true;
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
                    if(button - 1 == i - INPUT_MOUSELEFT) {
                        state_[i] = false;
                    }
                }
            }

            if(event_.type == SDL_KEYDOWN && event_.key.repeat == 0) {
                int scancode = event_.key.keysym.scancode;
                state_[scancode] = true;
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
                state_[scancode] = false;
                released_[scancode] = true;
                released_change_.push_back(scancode);
            }
            
            if(event_.type == SDL_TEXTINPUT) {
                text_input_ += event_.text.text;
            }
        }
    }

    void Inputs::reset_states() {
        for(int key : pressed_change_) {
            pressed_[key] = false;
        }
        for(int key : released_change_) {
            released_[key] = false;
        }
        pressed_change_.clear();
        released_change_.clear();
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

    bool Inputs::get_state(std::string command) {
        return get_state_raw(binds_[command]);
    }

    bool Inputs::get_pressed_raw(INPUT_CODE input) {
        return pressed_[input];
    }

    bool Inputs::get_released_raw(INPUT_CODE input) {
        return released_[input];
    }

    bool Inputs::get_state_raw(INPUT_CODE input) {
        return state_[input];
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
        return scaled_mouse_;
    }

    void Inputs::scale_mouse_pos(Vec2D window_dim, Vec2D logic_dim) {
        float scale_x = window_dim.x / logic_dim.x;
        float scale_y = window_dim.y / logic_dim.y;

        scaled_mouse_.x = mouse_x_ / scale_x;
        scaled_mouse_.y = mouse_y_ / scale_y;
    }

    bool Inputs::get_quit() {
        return quit_;
    }
}