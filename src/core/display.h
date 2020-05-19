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
        SDL_Renderer *renderer_;

        Vec2D logic_dim_;

    public:
        Display(int width, int height, std::string title, 
                bool fullscreen, bool vsync);
        ~Display();

        // Get the SDL window
        SDL_Window *get_window();

        // Get the SDL renderer
        SDL_Renderer *get_renderer();

        // Check if the display is on fullscreen mode
        bool is_fullscreen();

        // Get the physical GUI window dimentions
        Vec2D get_window_dimensions();

        // Get the logical dimensions of the renderer
        Vec2D get_dimensions();

        // Set the logical dimensions of the renderer
        void set_dimensions(int x, int y);

        // Set the GUI bar title
        void set_title(std::string title);

        // Toggle fullscreen mode
        void set_fullscreen(bool mode);

        // Set the cursor visibility
        void set_cursor_visibility(bool mode);
    };
}

#endif