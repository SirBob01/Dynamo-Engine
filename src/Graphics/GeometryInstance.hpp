#pragma once

#include "../Types.hpp"
#include "./Geometry.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Defines an array range.
     *
     */
    struct BufferRange {
        u32 offset;
        u32 count;
    };

    /**
     * @brief Instance of a geometry in the GPU buffer.
     *
     */
    class GeometryInstance {
      public:
        /**
         * @brief Destroy the GeometryInstance object.
         *
         */
        virtual ~GeometryInstance() = 0;

        /**
         * @brief Get the range of the vertex buffer.
         *
         * @return u32
         */
        virtual const BufferRange &get_vertex_range() const = 0;

        /**
         * @brief Get the range of the index buffer.
         *
         * @return u32
         */
        virtual const BufferRange &get_index_range() const = 0;
    };
} // namespace Dynamo::Graphics