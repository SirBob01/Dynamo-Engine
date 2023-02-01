#pragma once

#include "../Math/Matrix.hpp"
#include "./Material.hpp"
#include "./MeshInstance.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief A section of a Layer to draw onto.
     *
     */
    class LayerView {
      public:
        /**
         * @brief Destroy the LayerView object.
         *
         */
        virtual ~LayerView() = default;

        /**
         * @brief Draw instanced models.
         *
         * @param mesh_instance Mesh instance.
         * @param material      Model material.
         * @param transforms    Transform array.
         */
        virtual void draw(MeshInstance &mesh_instance,
                          Material &material,
                          std::vector<Mat4> &transforms) = 0;
    };
} // namespace Dynamo::Graphics