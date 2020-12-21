#include <rg/renderer/model/Transform.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat3x3.hpp>

namespace rg {

glm::mat4 Transform::get_model_matrix() const {
    glm::mat4 model{1.0f};
    model = glm::translate(model, position);
    model = glm::mat4_cast(orientation) * model;
    model = glm::scale(model, scale);
    return model;
}

glm::mat4 Transform::get_normal_matrix() const {
    return glm::inverse(glm::mat3{get_model_matrix()});
}

glm::mat4 Transform::get_translate_matrix() const {
    return glm::translate(glm::mat4{1.0f}, position);
}

glm::mat4 Transform::get_rotate_matrix() const {
    return glm::mat4_cast(orientation);
}

glm::mat4 Transform::get_scale_matrix() const {
    return glm::scale(glm::mat4{1.0f}, scale);
}

} // namespace rg
