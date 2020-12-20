#include <rg/renderer/camera/Surface.hpp>

#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>
#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/buffer/VertexLayout.hpp>
#include <rg/renderer/model/Texture.hpp>
#include <rg/util/layouts.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace rg {

Surface::Surface(unsigned int width, unsigned int height) : fb_{width, height} {
    // A [-1, 1]x[-1, 1] square, with texture coordinates mapping to [0,1]x[0,1]
    static const float vertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f, -1.0f, 1.0f,  0.0f, 1.0f,
    };
    static const unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    static const VertexLayout layout{rg::util::elementf(2),
                                     rg::util::elementf(2)};

    VertexArray va;
    va.bind();
    VertexBuffer vb{vertices, sizeof(vertices)};
    vb.bind();
    IndexBuffer ib{indices, sizeof(indices) / sizeof(*indices)};
    ib.bind();
    va.recordLayout(vb, layout);

    quad_ = new Mesh(std::make_shared<VertexArray>(std::move(va)),
                     std::make_shared<VertexBuffer>(std::move(vb)),
                     std::make_shared<IndexBuffer>(std::move(ib)),
                     {std::make_shared<Texture>(fb_.get_color_texture(),
                                                TextureType::DIFFUSE)});
}

Surface::~Surface() {
    delete quad_;
}

void Surface::draw(const Shader& shader,
                   const DrawDirectives& directives) const {
    shader.bind();
    shader.set("model", directives.get_model_matrix());
    shader.set("tex", directives.get_texture_matrix());
    quad_->draw(shader);
}

void Surface::draw(const Shader& shader) const {
    shader.bind();
    shader.set("model", glm::mat4{1.0f});
    shader.set("tex", glm::mat4{1.0f});
    quad_->draw(shader);
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
