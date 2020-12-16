#include <rg/renderer/light/PointLight.hpp>

namespace rg {

void PointLight::apply(Shader* shader, const std::string& fieldName) const {
    Light::apply(shader, fieldName);
    shader->set(fieldName + ".position", position_);
    shader->set(fieldName + ".constant", constant_);
    shader->set(fieldName + ".linear", linear_);
    shader->set(fieldName + ".quadratic", quadratic_);
}
std::string PointLight::getShaderFieldName() const {
    return "pointLights";
}
} // namespace rg