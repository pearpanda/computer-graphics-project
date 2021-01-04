#include <rg/renderer/render.hpp>

namespace rg {

void render(const Shader& shader, const Model& model,
            const Transform& transform) {
    shader.bind();
    shader.set("model_matrix", transform.get_model_matrix());
    model.draw(shader);
    shader.unbind();
}

void render(const Shader& shader, const Model& model,
            const Transform& transform, float shininess) {
    shader.bind();
    shader.set(transform);
    shader.set_float("material.shininess", shininess);
    model.draw(shader);
    shader.unbind();
}

void render(const Shader& skybox_shader, const Skybox& skybox) {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    skybox_shader.bind();
    skybox.draw(skybox_shader);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void render(const Shader& surface_shader, const Surface& surface) {
    // Draw to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    surface.draw(surface_shader);
}

void render(const Shader& surface_shader, const Surface& surface,
            unsigned int index) {
    // Draw to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // The following code directs the surfaces to be drawn in its place on
    // the screen:
    //
    // +-------------------+
    // |         |         |
    // |    4    |    3    |
    // |         |         |
    // +---------+---------+
    // |         |         |
    // |    1    |    2    |
    // |         |         |
    // +---------+---------+

    static const Surface::SubViewDirectives no_transform{glm::vec2{0.0f, 0.0f},
                                                         glm::vec2{1.0f, 1.0f}};
    static const glm::vec2 surface_dimensions{1.0f, 1.0f};
    static const std::array<glm::vec2, 4> origins{
            glm::vec2{-0.5f, -0.5f},
            glm::vec2{0.5f, -0.5f},
            glm::vec2{0.5f, 0.5f},
            glm::vec2{-0.5f, 0.5f},
    };
    static const std::array<Surface::DrawDirectives, 4> directives{
            Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                            origins[0], surface_dimensions},
                                    no_transform},
            Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                            origins[1], surface_dimensions},
                                    no_transform},
            Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                            origins[2], surface_dimensions},
                                    no_transform},
            Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                            origins[3], surface_dimensions},
                                    no_transform}};

    // Draw the current numbered surface
    surface.draw(surface_shader, directives[index]);
}

void clear() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void clear(const Surface& surface) {
    surface.bind();
    glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace rg
