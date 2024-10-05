#pragma once

#include <Display.hpp>
#include <Graphics/Vulkan/Debugger.hpp>
#include <Graphics/Vulkan/Instance.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan-powered 3D Renderer.
     *
     */
    class Renderer {
        std::unique_ptr<Instance> _instance;
        std::unique_ptr<Debugger> _debugger;

      public:
        /**
         * @brief Initialize the renderer.
         *
         * @param display
         */
        Renderer(const Display &display);
    };
} // namespace Dynamo::Graphics::Vulkan