#ifndef DYNAMO_INPUTS_H_
#define DYNAMO_INPUTS_H_

#include <SDL2/SDL.h>

#include <unordered_map>
#include <vector>
#include <array>
#include <string>

#include "../util/vector.h"

namespace Dynamo {
    enum class Input { 
        // Adapted from SDL_Scancode enum
        Unknown = 0,
        
        A = 4,
        B = 5,
        C = 6,
        D = 7,
        E = 8,
        F = 9,
        G = 10,
        H = 11,
        I = 12,
        J = 13,
        K = 14,
        L = 15,
        M = 16,
        N = 17,
        O = 18,
        P = 19,
        Q = 20,
        R = 21,
        S = 22,
        T = 23,
        U = 24,
        V = 25,
        W = 26,
        X = 27,
        Y = 28,
        Z = 29,
        K1 = 30,
        K2 = 31,
        K3 = 32,
        K4 = 33,
        K5 = 34,
        K6 = 35,
        K7 = 36,
        K8 = 37,
        K9 = 38,
        K0 = 39,
        Return = 40,
        Escape = 41,
        BackSpace = 42,
        Tab = 43,
        Space = 44,
        Minus = 45,
        Equals = 46,
        LeftBracket = 47,
        RightBracket = 48,
        BackSlash = 49,
        NonUsHash = 50,
        Semicolon = 51,
        Apostrophe = 52,
        Grave = 53,
        Comma = 54,
        Period = 55,
        Slash = 56,
        CapsLock = 57,
        F1 = 58,
        F2 = 59,
        F3 = 60,
        F4 = 61,
        F5 = 62,
        F6 = 63,
        F7 = 64,
        F8 = 65,
        F9 = 66,
        F10 = 67,
        F11 = 68,
        F12 = 69,
        PrintScreen = 70,
        ScrollLock = 71,
        Pause = 72,
        Insert = 73,
        Home = 74,
        PageUp = 75,
        Delete = 76,
        End = 77,
        PageDown = 78,
        Right = 79,
        Left = 80,
        Down = 81,
        Up = 82,
        NumLockClear = 83,
        KpDivide = 84,
        KpMultiply = 85,
        KpMinus = 86,
        KpPlus = 87,
        KpEnter = 88,
        Kp1 = 89,
        Kp2 = 90,
        Kp3 = 91,
        Kp4 = 92,
        Kp5 = 93,
        Kp6 = 94,
        Kp7 = 95,
        Kp8 = 96,
        Kp9 = 97,
        Kp0 = 98,
        KpPeriod = 99,
        NonUsBackSlash = 100,
        Application = 101,
        Power = 102,
        KpEquals = 103,
        F13 = 104,
        F14 = 105,
        F15 = 106,
        F16 = 107,
        F17 = 108,
        F18 = 109,
        F19 = 110,
        F20 = 111,
        F21 = 112,
        F22 = 113,
        F23 = 114,
        F24 = 115,
        Execute = 116,
        Help = 117,
        Menu = 118,
        Select = 119,
        Stop = 120,
        Again = 121,
        Undo = 122,
        Cut = 123,
        Copy = 124,
        Paste = 125,
        Find = 126,
        Mute = 127,
        VolumeUp = 128,
        VolumeDown = 129,
        KpComma = 133,
        KpEqualsAS400 = 134,
        International1 = 135,
        International2 = 136,
        International3 = 137,
        International4 = 138,
        International5 = 139,
        International6 = 140,
        International7 = 141,
        International8 = 142,
        International9 = 143,
        Lang1 = 144,
        Lang2 = 145,
        Lang3 = 146,
        Lang4 = 147,
        Lang5 = 148,
        Lang6 = 149,
        Lang7 = 150,
        Lang8 = 151,
        Lang9 = 152,
        AltErase = 153,
        SysReq = 154,
        Cancel = 155,
        Clear = 156,
        Prior = 157,
        Return2 = 158,
        Separator = 159,
        Out = 160,
        Oper = 161,
        ClearAgain = 162,
        CrSel = 163,
        ExSel = 164,
        Kp00 = 176,
        Kp000 = 177,
        ThousandsSeparator = 178,
        DecimalSeparator = 179,
        CurrencyUnit = 180,
        CurrencySubUnit = 181,
        KpLeftParen = 182,
        KpRightParen = 183,
        KpLeftBrace = 184,
        KpRightBrace = 185,
        KpTab = 186,
        KpBackSpace = 187,
        KpA = 188,
        KpB = 189,
        KpC = 190,
        KpD = 191,
        KpE = 192,
        KpF = 193,
        KpXOR = 194,
        KpPower = 195,
        KpPercent = 196,
        KpLess = 197,
        KpGreater = 198,
        KpAmpersand = 199,
        KpDblAmpersand = 200,
        KpVerticalBar = 201,
        KpDblVerticalBar = 202,
        KpColon = 203,
        KpHash = 204,
        KpSpace = 205,
        KpAt = 206,
        KpExclam = 207,
        KpMemStore = 208,
        KpMemRecall = 209,
        KpMemClear = 210,
        KpMemAdd = 211,
        KpMemSubtract = 212,
        KpMemMultiply = 213,
        KpMemDivide = 214,
        KpPlusMinus = 215,
        KpClear = 216,
        KpClearEntry = 217,
        KpBinary = 218,
        KpOctal = 219,
        KpDecimal = 220,
        KpHexadecimal = 221,
        LCtrl = 224,
        LShift = 225,
        LAlt = 226,
        LGUI = 227,
        RCtrl = 228,
        RShift = 229,
        RAlt = 230,
        RGUI = 231,
        Mode = 257,
        AudioNext = 258,
        AudioPrev = 259,
        AudioStop = 260,
        AudioPlay = 261,
        AudioMute = 262,
        MediaSelect = 263,
        WWW = 264,
        Mail = 265,
        Calculator = 266,
        Computer = 267,
        AcSearch = 268,
        AcHome = 269,
        AcBack = 270,
        AcForward = 271,
        AcStop = 272,
        AcRefresh = 273,
        AcBookmarks = 274,
        BrightnessDown = 275,
        BrightnessUp = 276,
        DisplaySwitch = 277,
        KbdIllumToggle = 278,
        KbdIllumDown = 279,
        KbdIllumUp = 280,
        Eject = 281,
        Sleep = 282,
        App1 = 283,
        App2 = 284,

        MouseLeft = 513,
        MouseMiddle = 514,
        MouseRight = 515,

        MaxCode = 516 // Not an input, just the highest value
    };

    enum class WindowEvent {
        None = -1,
        Shown = SDL_WINDOWEVENT_SHOWN,
        Hidden = SDL_WINDOWEVENT_HIDDEN,
        Exposed = SDL_WINDOWEVENT_EXPOSED,
        Move = SDL_WINDOWEVENT_MOVED,
        Resize = SDL_WINDOWEVENT_RESIZED,
        SizeChange = SDL_WINDOWEVENT_SIZE_CHANGED,
        Minimize = SDL_WINDOWEVENT_MINIMIZED,
        Maximize = SDL_WINDOWEVENT_MAXIMIZED,
        Restore = SDL_WINDOWEVENT_RESTORED,
        Enter = SDL_WINDOWEVENT_ENTER,
        Leave = SDL_WINDOWEVENT_LEAVE,
        FocusGain = SDL_WINDOWEVENT_FOCUS_GAINED,
        FocusLost = SDL_WINDOWEVENT_FOCUS_LOST,
        Close = SDL_WINDOWEVENT_CLOSE
    };

    class InputHandler {
        SDL_Event event_;

        using KeyMap = std::array<bool, static_cast<int>(Input::MaxCode)>;

        KeyMap pressed_;
        KeyMap released_;
        KeyMap state_;
        
        std::vector<int> pressed_change_;
        std::vector<int> released_change_;
        
        std::unordered_map<std::string, Input> binds_;
        std::string text_input_;

        Vec2D mouse_pos_;

        WindowEvent window_event_;

        bool recording_text_;
        bool quit_;

        // Reset pressed and released state arrays
        void reset_states();

    public:
        InputHandler();

        // Update the input handler
        void poll();

        // Check if recording text input
        bool is_recording_text();

        // Start recording text input
        void toggle_recording_text();
        
        // Reset the text input string field
        void clear_text();

        // Get the value of a text input event
        std::string get_text();

        // Bind a string command to an input
        void bind(std::string command, Input input);
        
        // Get the input mapped to a command
        Input get_bind(std::string command);

        // Check the pressed state of an input
        bool get_pressed(std::string command);

        // Check the released state of an input
        bool get_released(std::string command);

        // Get the state of the command bind (held down or released)
        bool get_state(std::string command);

        // Get the pressed state of a raw input constant
        bool get_pressed_raw(Input input);

        // Get the released state of a raw input constant
        bool get_released_raw(Input input);

        // Get the state of the button
        bool get_state_raw(Input input);

        // Get the name string of an input event
        std::string get_name(Input input);
        
        // Check for a window event
        WindowEvent get_window_event();

        // Get mouse position
        Vec2D get_mouse_pos();

        // Test if the event handler quit
        bool get_quit();

        // Send a quit signal
        void signal_quit();

        // Perform a function on each input code
        // Function signature: void f(Dynamo::Input);
        template <class F>
        void each_code(F func) {
            for(int i = 0; i < static_cast<int>(Input::MaxCode); i++) {
                Input code = static_cast<Input>(i);
                func(code);
            }
        };

        // Perform a function each command bind
        // Function signature: void f(std::string, Dynamo::Input);
        template <class F>
        void each_bind(F func) {
            for(auto &bind : binds_) {
                func(bind.first, bind.second);
            }
        }
    };
}

#endif