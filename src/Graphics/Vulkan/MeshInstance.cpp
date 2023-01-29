#include "./MeshInstance.hpp"

namespace Dynamo::Graphics::Vulkan {
    MeshInstance::MeshInstance(Mesh &mesh,
                               Buffer &object_buffer,
                               Buffer &staging_buffer) :
        _object_buffer(object_buffer) {
        u32 vertex_buffer_length =
            mesh.vertices.size() * sizeof(mesh.vertices[0]);
        u32 index_buffer_length =
            mesh.indices.size() * sizeof(mesh.indices[0]);

        u32 vs_offset = staging_buffer.reserve(vertex_buffer_length, 1);
        u32 is_offset = staging_buffer.reserve(index_buffer_length, 1);

        u32 vo_offset =
            _object_buffer.get().reserve(vertex_buffer_length, 1);
        u32 io_offset =
            _object_buffer.get().reserve(index_buffer_length, 1);

        // Write mesh data to the staging buffer
        staging_buffer.write(reinterpret_cast<u8 *>(mesh.vertices.data()),
                             vs_offset,
                             vertex_buffer_length);
        staging_buffer.write(reinterpret_cast<u8 *>(mesh.indices.data()),
                             is_offset,
                             index_buffer_length);

        // Copy to the object buffer
        staging_buffer.copy(_object_buffer.get(),
                            vs_offset,
                            vo_offset,
                            vertex_buffer_length);
        staging_buffer.copy(_object_buffer.get(),
                            is_offset,
                            io_offset,
                            index_buffer_length);

        // Free up the staging buffer
        staging_buffer.free(vs_offset);
        staging_buffer.free(is_offset);

        // Update array buffer offsets and counts
        _vertex_count = mesh.vertices.size();
        _vertex_offset = vo_offset;

        _index_count = mesh.indices.size();
        _index_offset = io_offset;
    }

    MeshInstance::~MeshInstance() {
        _object_buffer.get().free(_vertex_offset);
        _object_buffer.get().free(_index_offset);
    }
} // namespace Dynamo::Graphics::Vulkan