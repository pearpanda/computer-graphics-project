#include <rg/renderer/render.hpp>

namespace rg {

void render(const Shader& shader, const Model& model, const View& eye,
            const Surface& surface, const Transform& transform) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.2f, 0.2f, 1.0f);

    surface.bind();
    shader.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model_matrix{1.0f};
    glm::mat4 view_matrix = eye.get_view_matrix();
    glm::mat4 projection_matrix = eye.get_projection_matrix();

    shader.set("model_matrix", transform.get_model_matrix());
    shader.set("view_matrix", view_matrix);
    shader.set("projection_matrix", projection_matrix);

    model.draw(shader);

    surface.unbind();
    shader.unbind();
}

void render(const rg::Shader& skybox_shader, const rg::Skybox& skybox,
            const rg::View& eye, const rg::Surface& surface) {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    surface.bind();
    skybox_shader.bind();

    glm::mat4 view_matrix = glm::mat4{glm::mat3{eye.get_view_matrix()}};
    glm::mat4 projection_matrix = eye.get_projection_matrix();

    skybox_shader.set("view_matrix", view_matrix);
    skybox_shader.set("projection_matrix", projection_matrix);

    skybox.draw(skybox_shader);

    surface.unbind();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void render(const rg::Shader& surface_shader, const rg::Surface& surface) {
    // Draw to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    surface.draw(surface_shader);
}

void render(const rg::Shader& surface_shader, const rg::Surface& surface,
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

    static const rg::Surface::SubViewDirectives no_transform{
            glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}};
    static const glm::vec2 surface_dimensions{1.0f, 1.0f};
    static const std::array<glm::vec2, 4> origins{
            glm::vec2{-0.5f, -0.5f},
            glm::vec2{0.5f, -0.5f},
            glm::vec2{0.5f, 0.5f},
            glm::vec2{-0.5f, 0.5f},
    };
    static const std::array<rg::Surface::DrawDirectives, 4> directives{
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[0], surface_dimensions},
                                        no_transform},
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[1], surface_dimensions},
                                        no_transform},
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[2], surface_dimensions},
                                        no_transform},
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[3], surface_dimensions},
                                        no_transform}};

    // Draw the current numbered surface
    surface.draw(surface_shader, directives[index]);
}

} // namespace rg
