#include <rg/util/common_meshes.hpp>

namespace rg::util {

namespace {

std::shared_ptr<MeshVertexData> generate(const float* vertices,
                                         unsigned int vertices_size,
                                         const unsigned int* indices,
                                         unsigned int indices_count,
                                         const VertexLayout& layout) {
    std::shared_ptr<MeshVertexData> mesh_data =
            std::make_shared<MeshVertexData>();
    mesh_data->vertex_array.bind();

    mesh_data->vertex_buffer = VertexBuffer{vertices, vertices_size};
    mesh_data->vertex_buffer.bind();
    mesh_data->index_buffer = IndexBuffer{indices, indices_count};
    mesh_data->index_buffer.bind();

    mesh_data->vertex_array.recordLayout(mesh_data->vertex_buffer, layout);
    mesh_data->vertex_array.unbind();
    mesh_data->index_buffer.unbind();
    mesh_data->vertex_buffer.unbind();

    return mesh_data;
}

} // namespace

std::shared_ptr<MeshVertexData> surfaceQuad() {
    static const float vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
    };

    static const unsigned int indices[] = {0, 1, 2, 0, 2, 3};
    static const VertexLayout layout{rg::util::elementf(2),
                                     rg::util::elementf(2)};

    return generate(vertices, sizeof(vertices), indices,
                    sizeof(indices) / sizeof(*indices), layout);
}

std::shared_ptr<MeshVertexData> fullCube() {
    static const float vertices[] = {-1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
                                     1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,
                                     -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
                                     1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f};
    static const unsigned int indices[] = {0, 1, 2, 0, 2, 3, 0, 7, 6, 0, 6, 1,
                                           0, 3, 4, 0, 4, 7, 5, 4, 3, 5, 3, 2,
                                           5, 2, 1, 5, 1, 6, 5, 6, 7, 5, 7, 4};

    static const VertexLayout layout{rg::util::elementf(3)};
    return generate(vertices, sizeof(vertices), indices,
                    sizeof(indices) / sizeof(*indices), layout);
}

} // namespace rg::util
