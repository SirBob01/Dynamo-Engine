#ifndef DYNAMO_GUI_WIDGET_H_
#define DYNAMO_GUI_WIDGET_H_

#include "../scene.h"
#include "../inputs.h"
#include "../vec2d.h"
#include "../aabb.h"

namespace Dynamo::GUI {
    typedef enum {
        WIDGET_DEFAULT,
        WIDGET_HOVER,
        WIDGET_CLICK
    } WIDGET_STATE;

    class Widget {    
    protected:
        AABB bounds_;
        Vec2D click_pos_;

        WIDGET_STATE state_;
        bool selected_;

    public:
        Widget();
        
        // Check if a point (e.g. mouse cursor) is within bounds
        bool is_in_bounds(Vec2D point);

        // Get state of the widget
        WIDGET_STATE get_state();

        // Get selected status of widget
        bool get_selected();

        // Center and resize the button
        void set_target(Vec2D pos, Vec2D dim);

        // Update the state of the widget
        void update(Inputs *inputs);
    };
}

#endif