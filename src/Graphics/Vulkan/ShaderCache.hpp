#pragma once

#include <functional>

#include "../../Asset/AssetCache.hpp"
#include "../Shader.hpp"
#include "./Device.hpp"
#include "./Shader.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan shader cache
     *
     */
    class ShaderCache : public AssetCache<Dynamo::Graphics::Shader> {
        std::reference_wrapper<Device> _device;

        /**
         * @brief Load a shader from a compiled bytecode file
         *
         * @param filepath
         * @return Dynamo::Graphics::Shader*
         */
        Dynamo::Graphics::Shader *load(std::string filepath) override;

      public:
        /**
         * @brief Construct a new ShaderCache object
         * 
         * @param asset_directory Root asset directory
         * @param device          Reference to the logical device
         */
        ShaderCache(const std::string asset_directory, Device &device);

        /**
         * @brief Destroy the ShaderCache object
         * 
         */
        virtual ~ShaderCache() = default;
    };
} // namespace Dynamo::Graphics::Vulkan