#pragma once

#include <functional>

#include "../Core/Display.hpp"
#include "../Math/Color.hpp"
#include "./Geometry.hpp"
#include "./GeometryInstance.hpp"
#include "./Target.hpp"
#include "./Texture.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Abstract graphics rendering engine for drawing 2D and 3D objects.
     *
     */
    class Renderer {
      public:
        /**
         * @brief Destroy the Renderer object.
         *
         */
        virtual ~Renderer() = 0;

        /**
         * @brief Upload geometry to the GPU and get a handle to its instance.
         *
         * @param geometry
         * @return std::unique_ptr<GeometryInstance>
         */
        virtual std::unique_ptr<GeometryInstance>
        add_geometry(Geometry &geometry) = 0;

        /**
         * @brief Upload a shader program to the GPU and get a handle to its
         * instance.
         *
         * @param shader_code
         * @param stage
         * @return std::unique_ptr<Shader>
         */
        virtual std::unique_ptr<Shader>
        add_shader(const std::string shader_code, ShaderStage stage) = 0;

        /**
         * @brief Upload a texture to the GPU and get a handle to the resulting
         * render target.
         *
         * @param texture
         * @return std::unique_ptr<Target>
         */
        virtual std::unique_ptr<Target> add_target(Texture &texture) = 0;

        /**
         * @brief Create a blank render target.
         *
         * @return std::unique_ptr<Target>
         */
        virtual std::unique_ptr<Target> add_target() = 0;

        /**
         * @brief Get the list of render targets.
         *
         * @return std::vector<std::unique_ptr<Target>>&
         */
        virtual std::vector<std::unique_ptr<Target>> &get_targets() = 0;

        /**
         * @brief Perform the rendering operations.
         *
         */
        virtual void refresh() = 0;
    };
} // namespace Dynamo::Graphics