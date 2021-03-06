#include "widget.h"

namespace Dynamo::GUI {
    Widget::Widget() {
        bounds_ = {
            {0, 0},
            {0, 0}
        };
        state_ = WIDGET_DEFAULT;
        selected_ = false;
    }

    bool Widget::is_in_bounds(Vec2D point) {
        return bounds_.is_in_bounds(point);
    }

    Vec2D Widget::get_position() {
        return bounds_.center;
    }

    WIDGET_STATE Widget::get_state() {
        return state_;
    }

    bool Widget::get_selected() {
        return selected_;
    }

    void Widget::set_state(WIDGET_STATE state) {
        state_ = state;
    }

    void Widget::set_target(Vec2D pos, Vec2D dim) {
        bounds_.center = pos;
        bounds_.dim = dim;
    }

    void Widget::update(InputHandler *inputs) {
        bool mouse_bounds = is_in_bounds(inputs->get_mouse_pos());
        bool click = inputs->get_state_raw(Input::MouseLeft);
        
        // Handle
        if(click) {
            if(mouse_bounds) {
                state_ = WIDGET_CLICK;
                selected_ = true;
            }
            else {
                state_ = WIDGET_DEFAULT;
                selected_ = false;
            }   
        }
        else {
            if(mouse_bounds) {
                state_ = WIDGET_HOVER;
            }
            else {
                state_ = WIDGET_DEFAULT;
            }
        }
    }
}