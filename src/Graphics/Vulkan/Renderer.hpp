#pragma once
#define GLFW_INCLUDE_VULKAN

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <stb_image.h>
#include <vulkan/vulkan.hpp>

#include "../../Log/Log.hpp"
#include "../../Math/Matrix.hpp"
#include "../../Types.hpp"
#include "../Material.hpp"
#include "../Mesh.hpp"
#include "../MeshCache.hpp"
#include "../MeshInstance.hpp"
#include "../Renderer.hpp"

#include "./Buffer.hpp"
#include "./CommandPool.hpp"
#include "./Debugger.hpp"
#include "./DescriptorPool.hpp"
#include "./Device.hpp"
#include "./DrawSet.hpp"
#include "./Fence.hpp"
#include "./Image.hpp"
#include "./Material.hpp"
#include "./MaterialSystem.hpp"
#include "./MemoryPool.hpp"
#include "./MeshInstance.hpp"
#include "./PhysicalDevice.hpp"
#include "./Pipeline.hpp"
#include "./PipelineLayout.hpp"
#include "./RenderPass.hpp"
#include "./Sampler.hpp"
#include "./Semaphore.hpp"
#include "./ShaderCache.hpp"
#include "./Swapchain.hpp"
#include "./Texture.hpp"
#include "./TextureCache.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Maximum number of frames in flight
     *
     */
    constexpr u32 MAX_FRAMES_IN_FLIGHT = 2;

    /**
     * @brief Renderer powered by the Vulkan API
     *
     */
    class Renderer : public Dynamo::Graphics::Renderer {
        vk::UniqueInstance _instance;
        vk::UniqueSurfaceKHR _surface;

        std::unique_ptr<Device> _device;
        std::unique_ptr<Sampler> _sampler;

        // Pools
        std::unique_ptr<MemoryPool> _memory_pool;
        std::unique_ptr<DescriptorPool> _descriptor_pool;
        std::unique_ptr<CommandPool> _graphics_command_pool;
        std::unique_ptr<CommandPool> _transfer_command_pool;

        // Command queues
        vk::Queue _graphics_queue;
        vk::Queue _transfer_queue;
        vk::Queue _present_queue;

        // Synchronizers
        std::vector<std::unique_ptr<Semaphore>> _signal_image_ready;
        std::vector<std::unique_ptr<Semaphore>> _signal_render_done;
        std::vector<std::unique_ptr<Fence>> _fences;

        // Buffers
        std::unique_ptr<Buffer> _staging_buffer;
        std::unique_ptr<Buffer> _object_buffer;
        std::unique_ptr<Buffer> _uniform_buffer;

        // Asset caches
        std::unique_ptr<MeshCache> _mesh_assets;
        std::unique_ptr<TextureCache> _texture_assets;
        std::unique_ptr<ShaderCache> _shader_assets;

        // Swapchain
        std::unique_ptr<Swapchain> _swapchain;

        // Material system
        std::unique_ptr<MaterialSystem> _material_system;

        // Mesh instances to draw, grouped by material
        std::unique_ptr<DrawSet> _draw_set;

        // Frame counter
        u32 _frame = 0;

        /**
         * @brief Debugger
         *
         */
        std::unique_ptr<Debugger> _debugger;

        /**
         * @brief List of supported extensions
         *
         */
        std::vector<const i8 *> _extensions;

        /**
         * @brief List of enabled validation layers for debugging
         *
         */
        std::vector<const i8 *> _validation_layers;

        /**
         * @brief Enumerate all the supported extensions
         *
         */
        void enumerate_extensions();

        /**
         * @brief Check if the system supports the enabled validation layers
         *
         * @return true
         * @return false
         */
        b8 is_supporting_layers();

        /**
         * @brief Create the Vulkan instance
         *
         */
        void create_instance();

        /**
         * @brief Attach the GLFW window to a Vulkan surface
         *
         */
        void create_surface();

        /**
         * @brief Select a hardware device and create the logical device
         *
         */
        void create_device();

        /**
         * @brief Create the Vulkan constructs
         *
         */
        void create_objects();

        /**
         * @brief Create the swapchain and its dependents
         *
         */
        void create_swapchain();

      public:
        /**
         * @brief Construct a new Renderer object
         *
         * @param display
         * @param asset_directory
         */
        Renderer(Display &display, const std::string asset_directory);

        /**
         * @brief Destroy the Renderer object
         *
         */
        ~Renderer();

        /**
         * @brief Get the mesh assets
         *
         * @return AssetCache<Mesh>&
         */
        AssetCache<Mesh> &get_mesh_assets() override;

        /**
         * @brief Get the texture assets
         *
         * @return AssetCache<Dynamo::Graphics::Texture>&
         */
        AssetCache<Dynamo::Graphics::Texture> &get_texture_assets() override;

        /**
         * @brief Get the shader assets
         *
         * @return AssetCache<Dynamo::Graphics::Shader>&
         */
        AssetCache<Dynamo::Graphics::Shader> &get_shader_assets() override;

        /**
         * @brief Get the material system
         *
         * @return MaterialSystem&
         */
        Dynamo::Graphics::MaterialSystem &get_material_system() override;

        /**
         * @brief Upload a Mesh to the GPU and get its instance
         *
         * @param mesh Raw mesh vertex and index arrays
         * @return std::unique_ptr<Dynamo::Graphics::MeshInstance>
         */
        std::unique_ptr<Dynamo::Graphics::MeshInstance>
        upload_mesh(Mesh &mesh) override;

        /**
         * @brief Set the render layers
         *
         * @param layers Ordered list of layers
         */
        void set_layers(std::vector<Dynamo::Graphics::Layer> &layers) override;

        /**
         * @brief Draw an instanced mesh
         *
         * @param mesh_instance Mesh instance
         * @param material      Model material
         * @param transforms    Transform instances
         * @param layer         Render layer index
         */
        void draw(Dynamo::Graphics::MeshInstance &mesh_instance,
                  Dynamo::Graphics::Material &material,
                  std::vector<Mat4> &transforms,
                  u32 layer = 0) override;

        /**
         * @brief Clear the display with a color
         *
         */
        void clear(Color color) override;

        /**
         * @brief Present the final rendered image to the screen
         *
         */
        void refresh() override;
    };
} // namespace Dynamo::Graphics::Vulkan