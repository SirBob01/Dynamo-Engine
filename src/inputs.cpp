#include "inputs.h"

Dynamo::Inputs::Inputs() {
	text_input = "";
	quit = false;
}

void Dynamo::Inputs::poll() {
	std::memset(pressed, false, (INPUT_LEN + 1) * sizeof(bool));
	std::memset(released, false, (INPUT_LEN + 1) * sizeof(bool));
	
	SDL_GetMouseState(&mouse_x, &mouse_y);

	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			quit = true;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			pressed[INPUT_MOUSELEFT] = (event.button.button == SDL_BUTTON_LEFT);
			pressed[INPUT_MOUSEMIDDLE] = (event.button.button == SDL_BUTTON_MIDDLE);
			pressed[INPUT_MOUSERIGHT] = (event.button.button == SDL_BUTTON_RIGHT);
		}
		if(event.type == SDL_MOUSEBUTTONUP) {
			released[INPUT_MOUSELEFT] = (event.button.button == SDL_BUTTON_LEFT);
			released[INPUT_MOUSEMIDDLE] = (event.button.button == SDL_BUTTON_MIDDLE);
			released[INPUT_MOUSERIGHT] = (event.button.button == SDL_BUTTON_RIGHT);
		}
		if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			pressed[event.key.keysym.scancode] = true;

			// Special backspace event
			if(text_input.size()) {
				if(event.key.keysym.sym == SDLK_BACKSPACE) {
					text_input.erase(text_input.size()-1);
				}
			}
		}
		if(event.type == SDL_KEYUP) {
			released[event.key.keysym.scancode] = true;
		}
		if(event.type == SDL_TEXTINPUT) {
			text_input += event.text.text;
		}
	}
}

std::string Dynamo::Inputs::get_text_input() {
	return text_input;
}

void Dynamo::Inputs::reset_text_input() {
	text_input = "";
}

void Dynamo::Inputs::bind(std::string command, INPUT input) {
	binds[command] = input;
}

Dynamo::INPUT Dynamo::Inputs::get_bind(std::string command) {
	return binds[command];
}

bool Dynamo::Inputs::get_pressed(std::string command) {
	return get_pressed_raw(binds[command]);
}

bool Dynamo::Inputs::get_released(std::string command) {
	return get_released_raw(binds[command]);
}

bool Dynamo::Inputs::get_pressed_raw(INPUT input) {
	return pressed[input];
}

bool Dynamo::Inputs::get_released_raw(INPUT input) {
	return released[input];
}

std::string Dynamo::Inputs::get_name(INPUT input) {
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

int Dynamo::Inputs::get_mouse_x() {
	return mouse_x;
}

int Dynamo::Inputs::get_mouse_y() {
	return mouse_y;
}

bool Dynamo::Inputs::get_quit() {
	return quit;
}