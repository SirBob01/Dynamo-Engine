#include "./Mesh.hpp"

namespace Dynamo::Graphics {
    Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<u32> &indices) :
        vertices(vertices), indices(indices) {}
} // namespace Dynamo::Graphics