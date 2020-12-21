#include <rg/renderer/render.hpp>

#include <vector>

namespace rg {

void render(const Shader& shader, const Model& model, const View& eye,
            const Surface& surface, const Transform& transform) {
    surface.bind();
    shader.bind();

    shader.set("model_matrix", transform.get_model_matrix());
    shader.set(eye);

    model.draw(shader);

    surface.unbind();
    shader.unbind();
}

void render(const Shader& shader, const Model& model, const View& eye,
            const Surface& surface, const Transform& transform, float shininess,
            const std::vector<DirectionalLight>& directional_lights,
            const std::vector<PointLight>& point_lights,
            const std::vector<SpotLight>& spotlights) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.2f, 0.2f, 1.0f);

    surface.bind();
    shader.bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Vertex shader uniforms
    // ----------------------
    shader.set(transform);
    shader.set(eye);

    // Fragment shader uniforms
    // ------------------------
    shader.set_float("material.shininess", shininess);
    shader.set("camera.position", eye.position);
    shader.set("camera.direction", eye.direction);

    // Lights
    // ------
    shader.set_int("active_directional_lights",
                   static_cast<int>(directional_lights.size()));
    shader.set_int("active_point_lights",
                   static_cast<int>(point_lights.size()));
    shader.set_int("active_spotlights", static_cast<int>(spotlights.size()));
    for (unsigned int i = 0; i < directional_lights.size(); ++i)
        shader.set("directional_lights[" + std::to_string(i) + "]",
                   directional_lights[i]);
    for (unsigned int i = 0; i < point_lights.size(); ++i)
        shader.set("point_lights[" + std::to_string(i) + "]", point_lights[i]);
    for (unsigned int i = 0; i < spotlights.size(); ++i)
        shader.set("spotlights[" + std::to_string(i) + "]", spotlights[i]);

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
