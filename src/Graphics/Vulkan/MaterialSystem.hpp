#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Material.hpp"
#include "../MaterialSystem.hpp"
#include "../Shader.hpp"

#include "./Buffer.hpp"
#include "./DescriptorPool.hpp"
#include "./Device.hpp"
#include "./Material.hpp"
#include "./Pipeline.hpp"
#include "./PipelineCache.hpp"
#include "./RenderPass.hpp"
#include "./Shader.hpp"
#include "./Swapchain.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan material building system
     *
     */
    class MaterialSystem : public Dynamo::Graphics::MaterialSystem {
        std::reference_wrapper<Device> _device;
        std::reference_wrapper<Swapchain> _swapchain;
        std::reference_wrapper<DescriptorPool> _descriptor_pool;
        std::reference_wrapper<Buffer> _uniform_buffer;

        RenderPass _renderpass;
        PipelineCache _cache;

      public:
        /**
         * @brief Construct a new MaterialSystem object
         *
         * @param device          Reference to the logical device
         * @param swapchain       Reference to the swapchain
         * @param descriptor_pool Reference to the descriptor pool
         * @param uniform_buffer  Reference to the uniform buffer
         */
        MaterialSystem(Device &device,
                       Swapchain &swapchain,
                       DescriptorPool &descriptor_pool,
                       Buffer &uniform_buffer);

        /**
         * @brief Destroy the MaterialSystem object
         *
         */
        ~MaterialSystem() = default;

        /**
         * @brief Recreate the material system with the swapchain
         *
         * @param swapchain
         */
        void reset(Swapchain &swapchain);

        /**
         * @brief Build a new material from a shader set
         *
         * @param shaders      List of all shader filenames used in the pipeline
         * @param polygon_mode Polygon rendering mode
         * @return std::unique_ptr<Dynamo::Graphics::Material>
         */
        std::unique_ptr<Dynamo::Graphics::Material>
        build(Dynamo::Graphics::ShaderList &shaders,
              PolygonMode polygon_mode) override;
    };
} // namespace Dynamo::Graphics::Vulkan