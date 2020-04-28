#ifndef DYNAMO_GUI_WIDGET_H_
#define DYNAMO_GUI_WIDGET_H_

#include "../main/scene.h"
#include "../core/inputs.h"
#include "../util/vector.h"
#include "../util/aabb.h"

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

        // Get the position of the widget
        Vec2D get_position();

        // Get state of the widget
        WIDGET_STATE get_state();

        // Get selected status of widget
        bool get_selected();

        // Hard set the state of the widget
        void set_state(WIDGET_STATE state);

        // Center and resize the widget
        void set_target(Vec2D pos, Vec2D dim);

        // Update the state of the widget
        void update(Inputs *inputs);
    };
}

#endif