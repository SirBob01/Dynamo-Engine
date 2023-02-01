#include "./PipelineCache.hpp"

namespace Dynamo::Graphics::Vulkan {
    PipelineCache::PipelineCache(Device &device) : _device(device) {
        vk::PipelineCacheCreateInfo cache_info;
        cache_info.initialDataSize = 0;
        cache_info.pInitialData = nullptr;
        _handle = _device.get().get_handle().createPipelineCache(cache_info);
    }

    PipelineCache::~PipelineCache() {
        _device.get().get_handle().destroyPipelineCache(_handle);
    }

    const vk::PipelineCache &PipelineCache::get_handle() const {
        return _handle;
    }
} // namespace Dynamo::Graphics::Vulkan