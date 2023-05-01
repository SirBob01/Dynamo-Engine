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
        upload_geometry(Geometry &geometry) = 0;

        /**
         * @brief Upload a shader program to the GPU and get a handle to its
         * instance.
         *
         * @param shader_code
         * @param stage
         * @return std::unique_ptr<Shader>
         */
        virtual std::unique_ptr<Shader>
        upload_shader(const std::string shader_code, ShaderStage stage) = 0;

        /**
         * @brief Upload a texture to the GPU and get a handle to its instance.
         *
         * @param pixels
         * @param width
         * @param height
         * @return std::unique_ptr<Texture>
         */
        virtual std::unique_ptr<Texture>
        upload_texture(std::vector<u8> &pixels, u32 width, u32 height) = 0;

        /**
         * @brief Add a target to the renderer.
         *
         * @return std::unique_ptr<Target>
         */
        virtual std::unique_ptr<Target> add_target() = 0;

        /**
         * @brief Get the list of render targets, excluding textures.
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