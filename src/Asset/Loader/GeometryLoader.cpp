#include "./GeometryLoader.hpp"

namespace Dynamo {
    Graphics::Geometry GeometryLoader::load(const std::string filepath) {
        tinyobj::attrib_t attrib;

        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;

        b8 result = tinyobj::LoadObj(&attrib,
                                     &shapes,
                                     &materials,
                                     &warning,
                                     &error,
                                     filepath.c_str());
        if (!result) {
            Log::error("Could not load Obj file: {}, ", error, warning);
        }

        // Generate the geometry
        Graphics::Geometry geometry;
        std::unordered_map<Graphics::Vertex, u32> unique_vertices;
        for (const tinyobj::shape_t &shape : shapes) {
            for (const tinyobj::index_t &index : shape.mesh.indices) {
                Graphics::Vertex vertex;
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                if (index.normal_index >= 0) {
                    vertex.normal = {
                        attrib.vertices[3 * index.normal_index + 0],
                        attrib.vertices[3 * index.normal_index + 1],
                        attrib.vertices[3 * index.normal_index + 2],
                    };
                }

                if (index.vertex_index >= 0) {
                    vertex.texture = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                if (unique_vertices.count(vertex) == 0) {
                    unique_vertices[vertex] = geometry.vertices.size();
                    geometry.vertices.push_back(vertex);
                }
                geometry.indices.push_back(unique_vertices[vertex]);
            }
        }
        return geometry;
    }
} // namespace Dynamo