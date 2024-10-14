#include <Graphics/Vulkan/PipelineCache.hpp>

namespace Dynamo::Graphics::Vulkan {
    PipelineCache::PipelineCache(VkDevice device, const std::string &filename) : _device(device) {
        std::ifstream ifstream;
        ifstream.open(filename, std::ios::app | std::ios::binary);
        ifstream.seekg(0, ifstream.end);
        size_t size = ifstream.tellg();
        ifstream.seekg(0, ifstream.beg);
        std::vector<char> buffer(size);
        ifstream.read(buffer.data(), size);
        ifstream.close();

        VkPipelineCacheCreateInfo cache_info;
        cache_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        cache_info.initialDataSize = size;
        cache_info.pInitialData = buffer.data();
        VkResult_log("Create Pipeline Cache", vkCreatePipelineCache(_device, &cache_info, nullptr, &_disk_cache));

        _ofstream.open(filename, std::ios::trunc | std::ios::binary);
    }

    PipelinePass PipelineCache::build(GraphicsPipelineSettings settings) {
        PipelinePass result;

        // Render Pass
        auto pass_it = _pass_cache.find(settings.renderpass);
        if (pass_it != _pass_cache.end()) {
            result.pass = pass_it->second;
        } else {
            result.pass = VkRenderPass_create(_device, settings.renderpass);
            _pass_cache.emplace(settings.renderpass, result.pass);
        }

        // Pipeline
        auto pipeline_it = _graphics_cache.find(settings);
        if (pipeline_it != _graphics_cache.end()) {
            result.pipeline = pipeline_it->second;
        } else {
            result.pipeline = VkPipeline_create(_device, _disk_cache, result.pass, settings);
            _graphics_cache.emplace(settings, result.pipeline);
        }

        return result;
    }

    void PipelineCache::clear() {
        for (const auto &[key, pipeline] : _graphics_cache) {
            vkDestroyPipeline(_device, pipeline, nullptr);
        }
        _graphics_cache.clear();

        for (const auto &[key, pass] : _pass_cache) {
            vkDestroyRenderPass(_device, pass, nullptr);
        }
        _pass_cache.clear();
    }

    void PipelineCache::destroy() {
        write_to_disk();
        clear();
        vkDestroyPipelineCache(_device, _disk_cache, nullptr);
        _ofstream.close();
    }

    void PipelineCache::write_to_disk() {
        size_t size;
        vkGetPipelineCacheData(_device, _disk_cache, &size, nullptr);
        std::vector<char> buffer(size);
        vkGetPipelineCacheData(_device, _disk_cache, &size, buffer.data());

        _ofstream.write(buffer.data(), buffer.size());
    }
} // namespace Dynamo::Graphics::Vulkan