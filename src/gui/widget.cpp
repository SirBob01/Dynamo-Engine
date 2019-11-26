#include "widget.h"

namespace Dynamo::GUI {
    Widget::Widget() {
        bounds_ = {
            {0, 0},
            {0, 0}
        };
        state_ = WIDGET_DEFAULT;
        selected_ = false;

        on_hover_ = nullptr;
        on_click_ = nullptr;
    }

    bool Widget::is_in_bounds(Vec2D point) {
        return bounds_.is_in_bounds(point);
    }

    WIDGET_STATE Widget::get_state() {
        return state_;
    }

    bool Widget::get_selected() {
        return selected_;
    }

    void Widget::set_hover_call(void (* func)()) {
        on_hover_ = func;
    }

    void Widget::set_click_call(void (* func)()) {
        on_click_ = func;
    }

    void Widget::set_target(Vec2D pos, Vec2D dim) {
        bounds_.center = pos;
        bounds_.dim = dim;
    }

    void Widget::update(Inputs *inputs) {
        bool mouse_bounds = is_in_bounds(inputs->get_mouse_pos());
        bool click = inputs->get_state_raw(INPUT_MOUSELEFT);
        
        if(click) {
            if(mouse_bounds) {
                state_ = WIDGET_CLICK;
                selected_ = true;
                if(on_click_) {
                    on_click_();
                }
            }
            else {
                state_ = WIDGET_DEFAULT;
                selected_ = false;
            }   
        }
        else {
            if(mouse_bounds) {
                state_ = WIDGET_HOVER;
                if(on_hover_) {
                    on_hover_();
                }
            }
            else {
                state_ = WIDGET_DEFAULT;
            }
        }
    }
}