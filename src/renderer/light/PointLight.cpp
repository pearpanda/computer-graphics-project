#include <rg/renderer/light/PointLight.hpp>

namespace rg {

void PointLight::apply(const Shader& shader, const std::string& fieldName) {
    Light::apply(shader, fieldName);
    shader.set(fieldName + ".position_", position_);
    shader.set(fieldName + ".constant_", constant_);
    shader.set(fieldName + ".linear_", linear_);
    shader.set(fieldName + ".quadratic_", quadratic_);
}
} // namespace rg