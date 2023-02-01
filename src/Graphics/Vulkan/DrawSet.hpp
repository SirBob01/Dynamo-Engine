#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "../../Math/Color.hpp"
#include "./Buffer.hpp"
#include "./CommandPool.hpp"
#include "./Material.hpp"
#include "./MeshInstance.hpp"
#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Instanced draw group
     *
     */
    struct InstanceGroup {
        /**
         * @brief Mesh instance
         *
         */
        std::reference_wrapper<MeshInstance> mesh_instance;

        /**
         * @brief Number of instances
         *
         */
        u32 count;
    };

    /**
     * @brief Groups mesh instances by material to minimize draw calls
     *
     */
    class DrawSet {
        std::reference_wrapper<Swapchain> _swapchain;
        std::reference_wrapper<CommandPool> _command_pool;
        std::reference_wrapper<Buffer> _object_buffer;
        vk::Queue _queue;

        std::unordered_map<Material *, std::vector<InstanceGroup>> _set;

        std::vector<vk::UniqueCommandBuffer> _command_buffers;

        vk::ClearValue _color_clear;
        vk::ClearValue _depth_clear;

      public:
        /**
         * @brief Construct a new DrawSet object
         *
         * @param swapchain      Reference to the swapchain
         * @param command_pool   Reference to the command pool
         * @param object_buffer  Reference to the object buffer
         * @param graphics_queue Graphics command queue
         */
        DrawSet(Swapchain &swapchain,
                CommandPool &command_pool,
                Buffer &object_buffer,
                vk::Queue graphics_queue);

        /**
         * @brief Add a new mesh instance group to draw
         *
         * @param mesh_instance Mesh instance
         * @param material      3D model material
         * @param transforms    List of transforms for each instance
         */
        void add(MeshInstance &mesh_instance,
                 Material &material,
                 std::vector<Mat4> &transforms);

        /**
         * @brief Set the clear color
         *
         */
        void clear(Color &color);

        /**
         * @brief Get the command buffer at a frame
         *
         * @param frame
         * @return vk::CommandBuffer&
         */
        vk::CommandBuffer &get_command_buffer(u32 frame);

        /**
         * @brief Record the draw commands
         *
         */
        void record();
    };
} // namespace Dynamo::Graphics::Vulkan