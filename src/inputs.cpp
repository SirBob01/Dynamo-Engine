#include "inputs.h"

GameInputs::GameInputs() {
	text_input = "";
	quit = false;
}

void GameInputs::poll() {
	std::memset(pressed, false, (GAME_INPUT_LEN + 1) * sizeof(bool));
	std::memset(released, false, (GAME_INPUT_LEN + 1) * sizeof(bool));
	
	SDL_GetMouseState(&mouse_x, &mouse_y);

	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			quit = true;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN) {
			pressed[GAME_INPUT_MOUSELEFT] = (event.button.button == SDL_BUTTON_LEFT);
			pressed[GAME_INPUT_MOUSEMIDDLE] = (event.button.button == SDL_BUTTON_MIDDLE);
			pressed[GAME_INPUT_MOUSERIGHT] = (event.button.button == SDL_BUTTON_RIGHT);
		}
		if(event.type == SDL_MOUSEBUTTONUP) {
			released[GAME_INPUT_MOUSELEFT] = (event.button.button == SDL_BUTTON_LEFT);
			released[GAME_INPUT_MOUSEMIDDLE] = (event.button.button == SDL_BUTTON_MIDDLE);
			released[GAME_INPUT_MOUSERIGHT] = (event.button.button == SDL_BUTTON_RIGHT);
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

void GameInputs::reset_text_input() {
	text_input = "";
}

std::string GameInputs::get_text_input() {
	return text_input;
}

void GameInputs::bind(std::string command, GAME_INPUT input) {
	binds[command] = input;
}

GAME_INPUT GameInputs::get_bind(std::string command) {
	return binds[command];
}

bool GameInputs::get_pressed(GAME_INPUT input) {
	return pressed[input];
}

bool GameInputs::get_released(GAME_INPUT input) {
	return released[input];
}

bool GameInputs::get_pressed_bind(std::string command) {
	return get_pressed(binds[command]);
}

bool GameInputs::get_released_bind(std::string command) {
	return get_released(binds[command]);
}

std::string GameInputs::get_name(GAME_INPUT input) {
	std::string s = " ";
	if(input < GAME_INPUT_MOUSELEFT) {
		s = SDL_GetScancodeName(static_cast<SDL_Scancode>(input));
	}
	else {
		switch(input) {
			case GAME_INPUT_MOUSELEFT:
				s = "Left Mouse";
				break;
			case GAME_INPUT_MOUSEMIDDLE:
				s = "Middle Mouse";
				break;
			case GAME_INPUT_MOUSERIGHT:
				s = "Right Mouse";
				break;
			default:
				break;
		}
	}
	return s;
}

int GameInputs::get_mouse_x() {
	return mouse_x;
}

int GameInputs::get_mouse_y() {
	return mouse_y;
}

bool GameInputs::get_quit() {
	return quit;
}