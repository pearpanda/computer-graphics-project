#include <rg/renderer/camera/Surface.hpp>

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <rg/util/common_meshes.hpp>

namespace rg {

Surface::Surface(unsigned int width, unsigned int height)
        : fb_{width, height}, quad_{util::surfaceQuad(),
                                    {std::make_shared<Texture>(
                                            fb_.get_color_texture(),
                                            TextureType::DIFFUSE)}} {
}

Surface::Surface(unsigned int width, unsigned int height,
                 std::shared_ptr<MeshVertexData> quad)
        : fb_{width, height}, quad_{std::move(quad),
                                    {std::make_shared<Texture>(
                                            fb_.get_color_texture(),
                                            TextureType::DIFFUSE)}} {
}

void Surface::draw(const Shader& shader,
                   const DrawDirectives& directives) const {
    shader.bind();
    shader.set("model", directives.get_model_matrix());
    shader.set("tex", directives.get_texture_matrix());
    fb_.blit();
    quad_.draw(shader);
}

void Surface::draw(const Shader& shader) const {
    shader.bind();
    shader.set("model", glm::mat4{1.0f});
    shader.set("tex", glm::mat4{1.0f});
    fb_.blit();
    quad_.draw(shader);
}

void Surface::bind() const {
    fb_.bind();
}

void Surface::unbind() const {
    fb_.unbind();
}

glm::mat4 Surface::ScreenDirectives::get_model_matrix() const {
    glm::mat4 model{1.0f};
    model = glm::translate(model, glm::vec3{origin.x, origin.y, 0.0f});
    model = glm::scale(
            model, glm::vec3{dimensions.x / 2.0f, dimensions.y / 2.0f, 1.0f});
    return model;
}

glm::mat4 Surface::SubViewDirectives::get_texture_matrix() const {
    glm::mat4 tex{1.0f};
    tex = glm::translate(tex, glm::vec3{origin.x, origin.y, 0.0f});
    tex = glm::scale(tex, glm::vec3{dimensions.x, dimensions.y, 1.0f});
    return tex;
}

glm::mat4 Surface::DrawDirectives::get_model_matrix() const {
    return sd_.get_model_matrix();
}

glm::mat4 Surface::DrawDirectives::get_texture_matrix() const {
    return svd_.get_texture_matrix();
}

Surface::SubViewDirectives::SubViewDirectives(glm::vec2 origin,
                                              glm::vec2 dimensions)
        : origin{origin}, dimensions{dimensions} {
}

Surface::ScreenDirectives::ScreenDirectives(glm::vec2 origin,
                                            glm::vec2 dimensions)
        : origin{origin}, dimensions{dimensions} {
}

Surface::DrawDirectives::DrawDirectives(ScreenDirectives sd,
                                        SubViewDirectives svd)
        : sd_{sd}, svd_{svd} {
}

} // namespace rg
