#include <rg/renderer/model/Mesh.hpp>

#include <glad/glad.h>

#include <array>
#include <utility>

namespace rg {

Mesh::Mesh(std::shared_ptr<VertexArray> va, std::shared_ptr<VertexBuffer> vb,
           std::shared_ptr<IndexBuffer> ib,
           std::vector<std::shared_ptr<Texture>> textures)
        : va_{std::move(va)}, vb_{std::move(vb)}, ib_{std::move(ib)},
          textures_{std::move(textures)} {
}

void Mesh::draw(const Shader& shader) const {
    static const std::array<std::string, 2> type_to_id{"texture_diffuse",
                                                       "texture_specular"};
    shader.bind();

    static std::array<unsigned int, 2> type_count;
    type_count[0] = type_count[1] = 0;

    for (unsigned int i = 0; i < textures_.size(); ++i) {
        // Select active texture slot
        glActiveTexture(GL_TEXTURE0 + i);

        auto idx = static_cast<unsigned int>(textures_[i]->type);
        // Tell the GPU which slot the texture occupies
        shader.set(type_to_id[idx] + std::to_string(type_count[idx] + 1),
                   static_cast<int>(i));
        ++type_count[idx];

        // Bind the texture to the active slot
        glBindTexture(GL_TEXTURE_2D, textures_[i]->texture_id);
    }

    // Reset active texture
    glActiveTexture(GL_TEXTURE0);

    va_->bind();
    ib_->bind();
    glDrawElements(GL_TRIANGLES, ib_->count(), GL_UNSIGNED_INT, nullptr);
    va_->unbind();
    ib_->unbind();
    shader.unbind();
}

} // namespace rg
