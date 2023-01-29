#pragma once

#include "./Mesh.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Instance of a mesh in the GPU buffer
     *
     */
    class MeshInstance {
      protected:
        /**
         * @brief Offset of the vertex array in the GPU buffer
         *
         */
        u32 _vertex_offset;

        /**
         * @brief Number of vertices
         *
         */
        u32 _vertex_count;

        /**
         * @brief Offset of the index array in the GPU buffer
         *
         */
        u32 _index_offset;

        /**
         * @brief Number of indices
         *
         */
        u32 _index_count;

      public:
        /**
         * @brief Destroy the MeshInstance object
         *
         */
        virtual ~MeshInstance() = default;

        /**
         * @brief Get the number of vertices
         *
         * @return u32
         */
        inline u32 vertex_count() { return _vertex_count; }

        /**
         * @brief Get the offset of the vertex array
         *
         * @return u32
         */
        inline u32 vertex_offset() { return _vertex_offset; }

        /**
         * @brief Get the number of indices
         *
         * @return u32
         */
        inline u32 index_count() { return _index_count; }

        /**
         * @brief Get the offset of the index array
         *
         * @return u32
         */
        inline u32 index_offset() { return _index_offset; }
    };
} // namespace Dynamo::Graphics