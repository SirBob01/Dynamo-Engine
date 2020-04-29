#ifndef DYNAMO_DISPLAY_H_
#define DYNAMO_DISPLAY_H_

#include <SDL2/SDL.h>

#include <algorithm>
#include <string>

#include "../util/vector.h"
#include "../log/error.h"

namespace Dynamo {
    class Display {
        SDL_Window *window_;

    public:
        Display(int width, int height, std::string title, bool fullscreen);
        ~Display();

        // Check if the display is on fullscreen mode
        bool is_fullscreen();

        // Get the pointer to the main window
        SDL_Window *get_window();

        // Get the physical GUI window dimentions
        Vec2D get_window_dimensions();

        // Set the GUI bar title
        void set_title(std::string title);

        // Toggle fullscreen mode
        void set_fullscreen(bool mode);

        // Set the cursor visibility
        void set_cursor_visibility(bool mode);
    };
}

#endif