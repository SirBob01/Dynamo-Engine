#include "./TextureCache.hpp"

namespace Dynamo::Graphics::Vulkan {
    TextureCache::TextureCache(const std::string asset_directory,
                               Device &device,
                               MemoryPool &memory_pool,
                               Buffer &staging_buffer,
                               CommandPool &command_pool,
                               vk::Queue graphics_queue) :
        AssetCache<Dynamo::Graphics::Texture>(asset_directory),
        _device(device), _memory_pool(memory_pool),
        _staging_buffer(staging_buffer), _command_pool(command_pool),
        _graphics_queue(graphics_queue) {}

    Dynamo::Graphics::Texture *TextureCache::load(std::string filename) {
        i32 width, height, channels;
        stbi_uc *raw_data = stbi_load(filename.c_str(),
                                      &width,
                                      &height,
                                      &channels,
                                      STBI_rgb_alpha);
        std::vector<u8> pixels(width * height * STBI_rgb_alpha);
        std::copy(raw_data, raw_data + pixels.size(), pixels.data());
        stbi_image_free(raw_data);

        return new Texture(pixels,
                           width,
                           height,
                           _device.get(),
                           _memory_pool.get(),
                           _staging_buffer.get(),
                           _command_pool.get(),
                           _graphics_queue);
    }
} // namespace Dynamo::Graphics::Vulkan