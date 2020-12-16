#include <rg/renderer/light/PointLight.hpp>

namespace rg {

void PointLight::apply(const Shader& shader, const std::string& fieldName) {
    Light::apply(shader, fieldName);
    shader.set(fieldName + ".position", position);
    shader.set(fieldName + ".constant", constant);
    shader.set(fieldName + ".linear", linear);
    shader.set(fieldName + ".quadratic", quadratic);
}
} // namespace rg