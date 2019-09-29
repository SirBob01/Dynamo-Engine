#include "inputs.h"

namespace Dynamo {
	Inputs::Inputs() {
		text_input_ = "";
		quit_ = false;
	}

	void Inputs::poll() {
		std::memset(pressed_, false, (INPUT_LEN + 1) * sizeof(bool));
		std::memset(released_, false, (INPUT_LEN + 1) * sizeof(bool));
		
		SDL_GetMouseState(&mouse_x_, &mouse_y_);

		while(SDL_PollEvent(&event_)) {
			if(event_.type == SDL_QUIT) {
				quit_ = true;
			}
			
			if(event_.type == SDL_MOUSEBUTTONDOWN) {
				uint8_t button = event_.button.button;

				pressed_[INPUT_MOUSELEFT] = (button == SDL_BUTTON_LEFT);
				pressed_[INPUT_MOUSEMIDDLE] = (button == SDL_BUTTON_MIDDLE);
				pressed_[INPUT_MOUSERIGHT] = (button == SDL_BUTTON_RIGHT);
			}
			
			if(event_.type == SDL_MOUSEBUTTONUP) {
				uint8_t button = event_.button.button;
				
				released_[INPUT_MOUSELEFT] = (button == SDL_BUTTON_LEFT);
				released_[INPUT_MOUSEMIDDLE] = (button == SDL_BUTTON_MIDDLE);
				released_[INPUT_MOUSERIGHT] = (button == SDL_BUTTON_RIGHT);
			}

			if(event_.type == SDL_KEYDOWN && event_.key.repeat == 0) {
				pressed_[event_.key.keysym.scancode] = true;

				// Special backspace event
				if(text_input_.size()) {
					if(event_.key.keysym.sym == SDLK_BACKSPACE) {
						text_input_.erase(text_input_.size()-1);
					}
				}
			}

			if(event_.type == SDL_KEYUP) {
				released_[event_.key.keysym.scancode] = true;
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

	void Inputs::bind(std::string command, INPUT input) {
		binds_[command] = input;
	}

	INPUT Inputs::get_bind(std::string command) {
		return binds_[command];
	}

	bool Inputs::get_pressed(std::string command) {
		return get_pressed_raw(binds_[command]);
	}

	bool Inputs::get_released(std::string command) {
		return get_released_raw(binds_[command]);
	}

	bool Inputs::get_pressed_raw(INPUT input) {
		return pressed_[input];
	}

	bool Inputs::get_released_raw(INPUT input) {
		return released_[input];
	}

	std::string Inputs::get_name(INPUT input) {
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

	int Inputs::get_mouse_x() {
		return mouse_x_;
	}

	int Inputs::get_mouse_y() {
		return mouse_y_;
	}

	bool Inputs::get_quit() {
		return quit_;
	}
}