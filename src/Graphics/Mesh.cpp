#include "./Mesh.hpp"

namespace Dynamo::Graphics::Vulkan {
    Mesh::Mesh(const std::string filename, const File3D filetype) {
        switch (filetype) {
        case File3D::Obj:
            load_obj(filename);
            break;
        default:
            Log::error("Invalid file format.");
            break;
        }
    }

    void Mesh::load_obj(const std::string filename) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;

        b8 result = tinyobj::LoadObj(&attrib,
                                       &shapes,
                                       &materials,
                                       &warning,
                                       &error,
                                       filename.c_str());
        if (!result) {
            Log::error("Could not load Obj file: {}, ", error, warning);
        }

        // Calculate the index array
        std::unordered_map<Vertex, u32> unique_vertices;
        for (const tinyobj::shape_t &shape : shapes) {
            for (const tinyobj::index_t &index : shape.mesh.indices) {
                Vertex vert;
                vert.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                if (index.normal_index >= 0) {
                    vert.normal = {
                        attrib.vertices[3 * index.normal_index + 0],
                        attrib.vertices[3 * index.normal_index + 1],
                        attrib.vertices[3 * index.normal_index + 2],
                    };
                }

                if (index.vertex_index >= 0) {
                    vert.texture = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
                    };
                }

                if (unique_vertices.count(vert) == 0) {
                    unique_vertices[vert] = vertices.size();
                    vertices.push_back(vert);
                }
                indices.push_back(unique_vertices[vert]);
            }
        }
    }
} // namespace Dynamo::Graphics::Vulkan