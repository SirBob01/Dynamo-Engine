#include "./MeshCache.hpp"

namespace Dynamo::Graphics {
    MeshCache::MeshCache(const std::string asset_directory) :
        AssetCache<Mesh>(asset_directory) {}

    Mesh *MeshCache::load_obj(const std::string filepath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warning, error;

        bool result = tinyobj::LoadObj(&attrib,
                                       &shapes,
                                       &materials,
                                       &warning,
                                       &error,
                                       filepath.c_str());
        if (!result) {
            Log::error("Could not load Obj file: {}, ", error, warning);
        }

        // Calculate the vertex and index arrays
        std::unordered_map<Vertex, u32> unique_vertices;
        std::vector<Vertex> vertices;
        std::vector<u32> indices;
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
        return new Mesh(vertices, indices);
    }

    Mesh *MeshCache::load(const std::string filepath) {
        if (filepath.find(".obj") == filepath.length() - 4) {
            return load_obj(filepath);
        }
        Log::error("Invalid mesh file format.");
    }
} // namespace Dynamo::Graphics