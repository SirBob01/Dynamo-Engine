#pragma once

#include <functional>

#include <vulkan/vulkan.hpp>

#include "./Device.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Wrapper class for a Vulkan image sampler that pre-processes images
     * before they are accessed by the shader stages (e.g. bilinear filtering,
     * anisotropic filtering)
     *
     */
    class Sampler {
        vk::Sampler _handle;
        std::reference_wrapper<Device> _device;

      public:
        /**
         * @brief Construct a new Sampler object
         *
         * @param device  Reference to the logical device
         * @param max_lod Maximum level of detail
         */
        Sampler(Device &device, unsigned max_lod);

        /**
         * @brief Destroy the Sampler object
         *
         */
        ~Sampler();

        /**
         * @brief Get the handle to vk::Sampler
         *
         * @return const vk::Sampler&
         */
        const vk::Sampler &get_handle() const;
    };
} // namespace Dynamo::Graphics::Vulkan