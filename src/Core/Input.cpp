#include "Input.hpp"

namespace Dynamo {
    Input::Input(Display &display) : _display(display) {
        // Register input handling callbacks to update internal state
        glfwSetWindowUserPointer(_display.get_window(), &_state);

        // On cursor motion
        glfwSetCursorPosCallback(_display.get_window(),
                                 [](GLFWwindow *window, double x, double y) {
                                     InputState *state =
                                         static_cast<InputState *>(
                                             glfwGetWindowUserPointer(window));
                                     state->mouse_position.x = x;
                                     state->mouse_position.y = y;
                                 });

        // On scroll
        glfwSetScrollCallback(_display.get_window(),
                              [](GLFWwindow *window, double x, double y) {
                                  InputState *state = static_cast<InputState *>(
                                      glfwGetWindowUserPointer(window));
                                  state->scroll_offset.x = x;
                                  state->scroll_offset.y = y;
                              });

        // On key state change
        glfwSetKeyCallback(
            _display.get_window(),
            [](GLFWwindow *window,
               int key,
               int scancode,
               int action,
               int mods) {
                InputState *state =
                    static_cast<InputState *>(glfwGetWindowUserPointer(window));
                switch (action) {
                case GLFW_PRESS:
                    state->key_pressed.insert(static_cast<KeyCode>(key));
                    break;
                case GLFW_RELEASE:
                    state->key_released.insert(static_cast<KeyCode>(key));
                    break;
                default:
                    break;
                }
            });

        // On mouse button state change
        glfwSetMouseButtonCallback(
            _display.get_window(),
            [](GLFWwindow *window, int button, int action, int mods) {
                InputState *state =
                    static_cast<InputState *>(glfwGetWindowUserPointer(window));
                switch (action) {
                case GLFW_PRESS:
                    state->mouse_pressed.insert(static_cast<MouseCode>(button));
                    break;
                case GLFW_RELEASE:
                    state->mouse_released.insert(
                        static_cast<MouseCode>(button));
                    break;
                default:
                    break;
                }
            });
    }

    const Vec2 &Input::get_mouse_position() const {
        return _state.mouse_position;
    }

    const Vec2 &Input::get_scroll_offset() const {
        return _state.scroll_offset;
    }

    bool Input::is_pressed(KeyCode code) {
        return _state.key_pressed.count(code);
    }

    bool Input::is_pressed(MouseCode code) {
        return _state.mouse_pressed.count(code);
    }

    bool Input::is_released(KeyCode code) {
        return _state.key_released.count(code);
    }

    bool Input::is_released(MouseCode code) {
        return _state.mouse_released.count(code);
    }

    bool Input::is_down(KeyCode code) {
        return glfwGetKey(_display.get_window(), static_cast<int>(code)) ==
               GLFW_PRESS;
    }

    bool Input::is_down(MouseCode code) {
        return glfwGetMouseButton(_display.get_window(),
                                  static_cast<int>(code)) == GLFW_PRESS;
    }

    void Input::poll() {
        // Reset scroll state
        _state.scroll_offset.x = 0;
        _state.scroll_offset.y = 0;

        // Reset key states
        _state.key_pressed.clear();
        _state.key_released.clear();

        // Reset mouse button states
        _state.mouse_pressed.clear();
        _state.mouse_released.clear();

        glfwPollEvents();
    }
} // namespace Dynamo