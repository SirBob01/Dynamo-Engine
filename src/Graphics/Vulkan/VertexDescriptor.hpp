#pragma once

#include <array>

#include <vulkan/vulkan.hpp>

#include "../Vertex.hpp"

namespace Dynamo::Graphics::Vulkan {
    /**
     * @brief Binding description of a Vertex
     *
     */
    constexpr vk::VertexInputBindingDescription VERTEX_BINDING_DESCRIPTION =
        vk::VertexInputBindingDescription(
            0,             // Index in array of bindings
            sizeof(Vertex) // Stride (memory buffer traversal)
        );

    /**
     * @brief Attribute descriptions of a Vertex
     *
     */
    constexpr std::array<vk::VertexInputAttributeDescription, 4>
        VERTEX_ATTRIBUTE_DESCRIPTION = {
            vk::VertexInputAttributeDescription(0,
                                                0,
                                                vk::Format::eR32G32B32Sfloat,
                                                offsetof(Vertex, position)),
            vk::VertexInputAttributeDescription(1,
                                                0,
                                                vk::Format::eR32G32B32Sfloat,
                                                offsetof(Vertex, normal)),
            vk::VertexInputAttributeDescription(2,
                                                0,
                                                vk::Format::eR32G32Sfloat,
                                                offsetof(Vertex, texture)),
    };
} // namespace Dynamo::Graphics::Vulkan
