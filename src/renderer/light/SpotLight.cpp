#include <rg/renderer/light/SpotLight.hpp>

namespace rg {

void SpotLight::apply(Shader* shader, const std::string& fieldName) const {
    Light::apply(shader, fieldName);
    shader->set(fieldName + ".position", position_);
    shader->set(fieldName + ".constant", constant_);
    shader->set(fieldName + ".linear", linear_);
    shader->set(fieldName + ".quadratic", quadratic_);
    shader->set(fieldName + ".direction", direction_);
    shader->set(fieldName + ".cutOff", cut_off_);
    shader->set(fieldName + ".outerCutOff", outer_cut_off_);
}
std::string SpotLight::getShaderFieldName() const {
    return "spotLights";
}

} // namespace rg