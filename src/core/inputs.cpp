#include "inputs.h"

namespace Dynamo {
    InputHandler::InputHandler() {
        text_input_ = "";
        recording_text_ = false;
        quit_ = false;

        mouse_pos_ = {0, 0};
        window_event_ = WindowEvent::None;

        pressed_.fill(false);
        released_.fill(false);
        state_.fill(false);
    }

    void InputHandler::reset_states() {
        for(int key : pressed_change_) {
            pressed_[key] = false;
        }
        for(int key : released_change_) {
            released_[key] = false;
        }
        pressed_change_.clear();
        released_change_.clear();
    }

    void InputHandler::poll() {
        reset_states();
        
        int lmb = static_cast<int>(Input::MouseLeft);
        int rmb = static_cast<int>(Input::MouseRight);

        while(SDL_PollEvent(&event_)) {
            if(event_.type == SDL_QUIT) {
                signal_quit();
            }
            
            if(event_.type == SDL_WINDOWEVENT) {
                window_event_ = static_cast<WindowEvent>(
                    event_.window.event
                );
            }
            else {
                window_event_ = WindowEvent::None;
            }

            if(event_.type == SDL_MOUSEMOTION) {
                mouse_pos_.x = event_.motion.x;
                mouse_pos_.y = event_.motion.y;
            }

            if(event_.type == SDL_MOUSEBUTTONDOWN) {
                uint8_t button = event_.button.button;
                for(int i = lmb; i <= rmb; i++) {
                    pressed_[i] = (button - 1 == i - lmb);
                    if(pressed_[i]) {
                        pressed_change_.push_back(i);
                    }
                    if(button - 1 == i - lmb) {
                        state_[i] = true;
                    }
                }
            }
            
            if(event_.type == SDL_MOUSEBUTTONUP) {
                uint8_t button = event_.button.button;
                for(int i = lmb; i <= rmb; i++) {
                    released_[i] = (button - 1 == i - lmb);
                    if(released_[i]) {
                        released_change_.push_back(i);
                    }
                    if(button - 1 == i - lmb) {
                        state_[i] = false;
                    }
                }
            }

            if(event_.type == SDL_KEYDOWN) {
                if(event_.key.repeat == 0) {
                    int scancode = event_.key.keysym.scancode;
                    state_[scancode] = true;
                    pressed_[scancode] = true;
                    pressed_change_.push_back(scancode);

                }           
                // Special backspace event
                if(text_input_.size() && recording_text_) {
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
            
            if(event_.type == SDL_TEXTINPUT && recording_text_) {
                text_input_ += event_.text.text;
            }
        }
    }

    bool InputHandler::is_recording_text() {
        return recording_text_;
    }

    void InputHandler::toggle_recording_text() {
        recording_text_ = !recording_text_;
    }
    
    void InputHandler::clear_text() {
        text_input_ = "";
    }

    std::string InputHandler::get_text() {
        return text_input_;
    }
    
    void InputHandler::bind(std::string command, Input input) {
        binds_[command] = input;
    }

    Input InputHandler::get_bind(std::string command) {
        return binds_[command];
    }

    bool InputHandler::get_pressed(std::string command) {
        return get_pressed_raw(binds_[command]);
    }

    bool InputHandler::get_released(std::string command) {
        return get_released_raw(binds_[command]);
    }

    bool InputHandler::get_state(std::string command) {
        return get_state_raw(binds_[command]);
    }

    bool InputHandler::get_pressed_raw(Input input) {
        return pressed_[static_cast<int>(input)];
    }

    bool InputHandler::get_released_raw(Input input) {
        return released_[static_cast<int>(input)];
    }

    bool InputHandler::get_state_raw(Input input) {
        return state_[static_cast<int>(input)];
    }

    std::string InputHandler::get_name(Input input) {
        std::string s = " ";
        if(input < Input::MouseLeft) {
            s = SDL_GetScancodeName(static_cast<SDL_Scancode>(input));
        }
        else {
            switch(input) {
                case Input::MouseLeft:
                    s = "Left Mouse";
                    break;
                case Input::MouseMiddle:
                    s = "Middle Mouse";
                    break;
                case Input::MouseRight:
                    s = "Right Mouse";
                    break;
                default:
                    break;
            }
        }
        return s;
    }

    WindowEvent InputHandler::get_window_event() {
        return window_event_;
    }

    Vec2D InputHandler::get_mouse_pos() {
        return mouse_pos_;
    }

    bool InputHandler::get_quit() {
        return quit_;
    }

    void InputHandler::signal_quit() {
        quit_ = true;
    }
}