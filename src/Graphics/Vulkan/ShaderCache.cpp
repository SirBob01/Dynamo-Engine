#include "./ShaderCache.hpp"

namespace Dynamo::Graphics::Vulkan {
    ShaderCache::ShaderCache(const std::string asset_directory,
                             Device &device) :
        AssetCache<Dynamo::Graphics::Shader>(asset_directory),
        _device(device) {}

    Dynamo::Graphics::Shader *ShaderCache::load(std::string filename) {
        return new Shader(_device.get(), filename);
    }
} // namespace Dynamo::Graphics::Vulkan