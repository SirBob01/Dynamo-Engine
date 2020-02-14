#ifndef DYNAMO_LIGHT_H_
#define DYNAMO_LIGHT_H_

#include "../sprite/sprite.h"
#include "../util/vec2d.h"

namespace Dynamo {
    // Defines a light source
    struct Light {
        Vec2D position_;
        float radius_;

        float beam_angle_;
        float offset_angle_;

        Color color_; // Alpha channel denotes luminosity
    };

    // Edges generate shadows with light contact
    struct MaterialEdge {
        Vec2D start;
        Vec2D end;
        Color color_; // Alpha channel denotes translucency
    };

    // Manages all lights
    class ShadowMap {
        std::vector<MaterialEdge> edges_;
        std::vector<LightPoint> lights_;

    public:
        ShadowMap();
        ~ShadowMap();

        // Add an edge to the edge list
        void add_edge(MaterialEdge edge);

        // Add a new light source
        void add_light(Light light);

        // Update shadow polygons for each light point
        void update();

        // Clear all light sources
        void clear();
    };
}

#endif