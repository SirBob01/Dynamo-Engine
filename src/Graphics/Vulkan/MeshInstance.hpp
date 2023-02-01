#pragma once

#include <functional>

#include "../MeshInstance.hpp"
#include "./Buffer.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan MeshInstance implementation
     *
     */
    class MeshInstance : public Dynamo::Graphics::MeshInstance {
        std::reference_wrapper<Buffer> _object_buffer;

      public:
        /**
         * @brief Construct a new MeshInstance object
         *
         * @param mesh           Raw mesh vertex and index arrays
         * @param object_buffer  Object buffer
         * @param staging_buffer Staging buffer
         */
        MeshInstance(Mesh &mesh, Buffer &object_buffer, Buffer &staging_buffer);

        /**
         * @brief Destroy the MeshInstance object
         *
         */
        ~MeshInstance();
    };
} // namespace Dynamo::Graphics::Vulkan