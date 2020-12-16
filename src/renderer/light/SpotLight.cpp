#include <rg/renderer/light/SpotLight.hpp>

namespace rg {

void SpotLight::apply(const Shader& shader, const std::string& fieldName) {
    Light::apply(shader, fieldName);
    shader.set(fieldName + ".position", position);
    shader.set(fieldName + ".constant", constant);
    shader.set(fieldName + ".linear", linear);
    shader.set(fieldName + ".quadratic", quadratic);
    shader.set(fieldName + ".direction", direction);
    shader.set(fieldName + ".cutOff", cutOff);
    shader.set(fieldName + ".outerCutOff", outerCutOff);
}

} // namespace rg