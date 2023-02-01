#pragma once

#include <functional>
#include <string>

#include <vulkan/vulkan.hpp>

#include "../Material.hpp"

#include "./Buffer.hpp"
#include "./DescriptorPool.hpp"
#include "./Device.hpp"
#include "./Pipeline.hpp"
#include "./PipelineLayout.hpp"
#include "./RenderPass.hpp"
#include "./Shader.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan Material
     *
     */
    class Material : public Dynamo::Graphics::Material {
        std::reference_wrapper<Device> _device;
        std::reference_wrapper<RenderPass> _renderpass;

        PipelineLayout _layout;
        Pipeline _pipeline;

        std::vector<DescriptorSetGroup> _descriptor_set_groups;

      public:
        /**
         * @brief Construct a new Material object
         *
         * @param device          Reference to the logical device
         * @param descriptor_pool Reference to the descriptor pool
         * @param swapchain       Reference to the swapchain
         * @param renderpass      Reference to the renderpass
         * @param cache           Reference to the pipeline cache
         * @param uniform_buffer  Reference to the uniform buffer
         * @param shaders         Shader modules
         * @param polygon_mode    Polygon mode
         */
        Material(Device &device,
                 DescriptorPool &descriptor_pool,
                 Swapchain &swapchain,
                 RenderPass &renderpass,
                 PipelineCache &cache,
                 Buffer &uniform_buffer,
                 ShaderList &shaders,
                 vk::PolygonMode polygon_mode);

        /**
         * @brief Destroy the Material object
         *
         */
        ~Material() = default;

        /**
         * @brief Get the pipeline object
         *
         * @return const Pipeline&
         */
        const Pipeline &get_pipeline() const;

        /**
         * @brief Get the pipeline layout object
         *
         * @return const PipelineLayout&
         */
        const PipelineLayout &get_layout() const;

        /**
         * @brief Get the render pass object
         *
         * @return const RenderPass&
         */
        const RenderPass &get_renderpass() const;

        /**
         * @brief Get the descriptor set groups
         *
         * @return const std::vector<DescriptorSetGroup>&
         */
        const std::vector<DescriptorSetGroup> &
        get_descriptor_set_groups() const;

        /**
         * @brief Write host local data to a uniform
         *
         * @param field Name of the uniform variable within the shader
         * @param src   Source data
         * @param size  Size of the data
         */
        void set(const std::string field, u8 *src, u32 size) override;

        /**
         * @brief Hash function to uniquely identify the material
         *
         * @return u32
         */
        u32 hash() const;

        /**
         * @brief Equality operator
         *
         * @param rhs
         * @return true
         * @return false
         */
        bool operator==(const Material &rhs) const;
    };
} // namespace Dynamo::Graphics::Vulkan

/**
 * @brief Hash function implementation for Circle
 *
 * @tparam
 */
template <>
struct std::hash<Dynamo::Graphics::Vulkan::Material> {
    inline size_t
    operator()(const Dynamo::Graphics::Vulkan::Material &material) const {
        return material.hash();
    }
};
