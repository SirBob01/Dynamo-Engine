#pragma once

#include <functional>

#include "../Asset/AssetCache.hpp"
#include "../Core/Display.hpp"
#include "../Math/Color.hpp"
#include "../Math/Matrix.hpp"

#include "./Layer.hpp"
#include "./Material.hpp"
#include "./MaterialSystem.hpp"
#include "./MeshInstance.hpp"
#include "./Texture.hpp"

namespace Dynamo::Graphics {
    /**
     * @brief Abstract graphics rendering engine for drawing 2D and 3D objects
     *
     */
    class Renderer {
      protected:
        std::reference_wrapper<Display> _display;
        std::string _asset_directory;

      public:
        /**
         * @brief Construct a new Renderer object
         *
         * @param display
         * @param asset_directory
         */
        Renderer(Display &display, const std::string asset_directory) :
            _display(display), _asset_directory(asset_directory) {}

        /**
         * @brief Destroy the Renderer object
         *
         */
        virtual ~Renderer() = default;

        /**
         * @brief Get the mesh assets
         *
         * @return AssetCache<Mesh>&
         */
        virtual AssetCache<Mesh> &get_mesh_assets() = 0;

        /**
         * @brief Get the texture assets
         *
         * @return AssetCache<Texture>&
         */
        virtual AssetCache<Texture> &get_texture_assets() = 0;

        /**
         * @brief Get the shader assets
         *
         * @return AssetCache<Shader>&
         */
        virtual AssetCache<Shader> &get_shader_assets() = 0;

        /**
         * @brief Get the material system
         *
         * @return MaterialSystem&
         */
        virtual MaterialSystem &get_material_system() = 0;

        /**
         * @brief Upload a Mesh to the GPU and get its instance
         *
         * @param mesh Raw vertex and index arrays
         * @return std::unique_ptr<MeshInstance>
         */
        virtual std::unique_ptr<MeshInstance> upload_mesh(Mesh &mesh) = 0;

        /**
         * @brief Set the render layers
         *
         * @param layers Ordered list of layers
         */
        virtual void set_layers(std::vector<Layer> &layers);

        /**
         * @brief Draw instanced models
         *
         * @param mesh_instance Mesh instance
         * @param material      Model material
         * @param transforms    Transform array
         * @param layer         Render layer index
         */
        virtual void draw(MeshInstance &mesh_instance,
                          Material &material,
                          std::vector<Mat4> &transforms,
                          u32 layer) = 0;

        /**
         * @brief Clear the display with a color
         *
         * @param color Clear color
         */
        virtual void clear(Color color) = 0;

        /**
         * @brief Refresh the renderer to update the display
         *
         */
        virtual void refresh() = 0;
    };
} // namespace Dynamo::Graphics