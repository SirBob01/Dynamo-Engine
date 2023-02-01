#pragma once

#include <stb_image.h>
#include <vulkan/vulkan.hpp>

#include "../../Asset/AssetCache.hpp"
#include "../Texture.hpp"
#include "./Buffer.hpp"
#include "./CommandPool.hpp"
#include "./Device.hpp"
#include "./Texture.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan Texture cache
     *
     */
    class TextureCache : public AssetCache<Dynamo::Graphics::Texture> {
        std::reference_wrapper<Device> _device;
        std::reference_wrapper<MemoryPool> _memory_pool;
        std::reference_wrapper<Buffer> _staging_buffer;
        std::reference_wrapper<CommandPool> _command_pool;

        vk::Queue _graphics_queue;

      protected:
        /**
         * @brief Load a texture from a raw image file
         *
         * @param filepath Path to the image file
         * @return Dynamo::Graphics::Texture
         */
        Dynamo::Graphics::Texture *load(std::string filepath) override;

      public:
        /**
         * @brief Construct a new TextureCache object
         *
         * @param asset_directory Root asset directory
         * @param device          Reference to the logical device
         * @param memory_pool     Reference to the memory pool
         * @param staging_buffer  Reference to the staging buffer
         * @param command_pool    Reference to the graphics command pool
         * @param graphics_queue  Graphics command queue
         */
        TextureCache(const std::string asset_directory,
                     Device &device,
                     MemoryPool &memory_pool,
                     Buffer &staging_buffer,
                     CommandPool &command_pool,
                     vk::Queue graphics_queue);

        /**
         * @brief Destroy the TextureCache object
         *
         */
        virtual ~TextureCache() = default;
    };
} // namespace Dynamo::Graphics::Vulkan