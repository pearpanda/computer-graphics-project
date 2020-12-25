#include <rg/renderer/model/Mesh.hpp>

#include <glad/glad.h>

#include <array>
#include <utility>

namespace rg {

Mesh::Mesh(std::shared_ptr<MeshVertexData> vertices,
           std::vector<std::shared_ptr<Texture>> textures)
        : vertices_{std::move(vertices)}, textures_{std::move(textures)} {
}

void Mesh::draw(const Shader& shader) const {
    static const std::array<std::string, 2> type_to_id{
            "material.texture_diffuse", "material.texture_specular"};
    shader.bind();

    static std::array<unsigned int, 2> type_count;
    type_count[0] = type_count[1] = 0;

    for (unsigned int i = 0; i < textures_.size(); ++i) {
        // Select active texture slot
        glActiveTexture(GL_TEXTURE0 + i);

        auto idx = static_cast<unsigned int>(textures_[i]->type);
        // Tell the GPU which slot the texture occupies
        shader.set_int(type_to_id[idx] + std::to_string(type_count[idx] + 1),
                       static_cast<int>(i));
        ++type_count[idx];

        // Bind the texture to the active slot
        glBindTexture(GL_TEXTURE_2D, textures_[i]->texture_id);
    }

    // Reset active texture
    glActiveTexture(GL_TEXTURE0);

    vertices_->vertex_array.bind();
    vertices_->index_buffer.bind();
    glDrawElements(GL_TRIANGLES, vertices_->index_buffer.count(),
                   GL_UNSIGNED_INT, nullptr);
    vertices_->vertex_array.unbind();
    vertices_->index_buffer.unbind();
}

} // namespace rg
