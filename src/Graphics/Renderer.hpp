#pragma once

#include <memory>

#include <Display.hpp>
#include <Graphics/Vulkan/Debugger.hpp>
#include <Graphics/Vulkan/Instance.hpp>
#include <Graphics/Vulkan/Surface.hpp>

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Vulkan-powered 3D Renderer.
     *
     */
    class Renderer {
        std::unique_ptr<Instance> _instance;
        std::unique_ptr<Debugger> _debugger;
        std::unique_ptr<Surface> _surface;

      public:
        /**
         * @brief Initialize the renderer.
         *
         * @param display
         */
        Renderer(const Display &display);
    };
} // namespace Dynamo::Graphics::Vulkan