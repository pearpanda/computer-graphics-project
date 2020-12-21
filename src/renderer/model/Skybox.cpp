#include <rg/renderer/model/Skybox.hpp>

#include <glad/glad.h>
#include <rg/util/common_meshes.hpp>

namespace rg {

Skybox::Skybox(const std::string& path, const std::vector<std::string>& faces)
        : Skybox{path, faces, util::fullCube()} {
}

Skybox::Skybox(const std::string& path, const std::vector<std::string>& faces,
               std::shared_ptr<MeshVertexData> cube)
        : cube_{std::move(cube)}, cubemap_{path, faces} {
}

void Skybox::draw(const Shader& shader) const {
    shader.bind();
    glActiveTexture(GL_TEXTURE0);
    shader.set_int("skybox", 0);
    cubemap_.bind();
    cube_->vertex_array.bind();
    cube_->index_buffer.bind();
    glDrawElements(GL_TRIANGLES, cube_->index_buffer.count(), GL_UNSIGNED_INT,
                   nullptr);
    cube_->vertex_array.unbind();
    cube_->index_buffer.unbind();
}

} // namespace rg
