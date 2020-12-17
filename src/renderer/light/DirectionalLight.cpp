#include <rg/renderer/light/DirectionalLight.hpp>

namespace rg {

DirectionalLight::DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse,
                                   glm::vec3 specular, glm::vec3 direction)
        : Light{ambient, diffuse, specular}, direction_{direction} {
}

void DirectionalLight::apply(Shader* shader,
                             const std::string& fieldName) const {
    Light::apply(shader, fieldName);
    shader->set(fieldName + ".direction", direction_);
}
std::string DirectionalLight::getShaderFieldName() const {
    return "dirLights";
}

DirectionalLight::Builder::Builder(const glm::vec3& direction)
        : b_direction_{direction} {
}

DirectionalLight* DirectionalLight::Builder::build() {
    return new DirectionalLight{b_ambient_, b_diffuse_, b_specular_,
                                b_direction_};
}
} // namespace rg