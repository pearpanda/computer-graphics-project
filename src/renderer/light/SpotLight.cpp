#include <rg/renderer/light/SpotLight.hpp>

namespace rg {

void SpotLight::apply(const Shader& shader, const std::string& fieldName) {
    Light::apply(shader, fieldName);
    shader.set(fieldName + ".position_", position_);
    shader.set(fieldName + ".constant_", constant_);
    shader.set(fieldName + ".linear_", linear_);
    shader.set(fieldName + ".quadratic_", quadratic_);
    shader.set(fieldName + ".direction_", direction_);
    shader.set(fieldName + ".cut_off_", cut_off_);
    shader.set(fieldName + ".outer_cut_off_", outer_cut_off_);
}

} // namespace rg