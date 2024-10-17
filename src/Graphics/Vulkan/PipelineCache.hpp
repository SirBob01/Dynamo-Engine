#pragma once

#include <fstream>
#include <unordered_map>

#include <vulkan/vulkan_core.h>

#include <Graphics/Material.hpp>
#include <Graphics/Vulkan/ShaderSet.hpp>
#include <Graphics/Vulkan/Utils.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Render pipeline pass object.
     *
     */
    struct PipelinePass {
        VkRenderPass pass;
        VkPipeline pipeline;
    };

    /**
     * @brief Pipeline cache to minimize expensive builds.
     *
     */
    class PipelineCache {
        VkDevice _device;
        std::ofstream _ofstream;
        VkPipelineCache _disk_cache;

        std::unordered_map<RenderPassSettings, VkRenderPass> _pass_cache;
        std::unordered_map<GraphicsPipelineSettings, VkPipeline> _graphics_cache;

      public:
        PipelineCache(VkDevice device, const std::string &filename);
        PipelineCache() = default;

        /**
         * @brief Build a pipeline pass.
         *
         * @param settings
         * @return PipelinePass
         */
        PipelinePass build(const GraphicsPipelineSettings &settings);

        /**
         * @brief Build a pipeline pass from a material.
         *
         * @param material
         * @param shader_set
         * @param layout
         * @param color_format
         * @return PipelinePass
         */
        PipelinePass
        build_material(const Material &material, ShaderSet &shader_set, VkPipelineLayout layout, VkFormat color_format);

        /**
         * @brief Clear all pipelines in the cache, invalidating all handles.
         *
         */
        void clear();

        /**
         * @brief Destroy the cache, invalidating all handles.
         *
         */
        void destroy();

        /**
         * @brief Write the pipeline cache to disk.
         *
         */
        void write_to_disk();
    };
} // namespace Dynamo::Graphics::Vulkan
