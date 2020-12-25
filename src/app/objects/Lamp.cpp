#include <app/objects/Lamp.hpp>

namespace app {

void Lamp::placeLight() const {
    auto model_matrix = transform.get_model_matrix();
    auto normal_matrix = transform.get_normal_matrix();
    spotlight->position =
            glm::vec3{model_matrix * glm::vec4{0.65651f, 3.905f, 0.0f, 1.0f}};
    float angle = glm::radians(-30.0f);
    spotlight->direction = glm::normalize(
            glm::vec3{normal_matrix *
                      glm::vec4{glm::cos(angle), glm::sin(angle), 0.0f, 1.0f}});
}
glm::vec3 Lamp::get_color() const {
    const auto& a = spotlight->color.ambient;
    const auto& d = spotlight->color.diffuse;
    const auto& s = spotlight->color.specular;

    float na = glm::length(a);
    float nd = glm::length(d);
    float ns = glm::length(s);

    return (a + d + s) / (na + nd + ns);
}

} // namespace app
