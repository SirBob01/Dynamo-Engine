#include <Graphics/Renderer.hpp>

namespace Dynamo::Graphics::Vulkan {
    Renderer::Renderer(const Display &display) {
        _instance = std::make_unique<Instance>(display);
#ifdef DYN_DEBUG
        _debugger = std::make_unique<Debugger>(*_instance);
#endif
    }
} // namespace Dynamo::Graphics::Vulkan