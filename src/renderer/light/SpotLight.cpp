#include <rg/renderer/light/SpotLight.hpp>

namespace rg {

SpotLight::SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                     glm::vec3 position, glm::vec3 direction, float cutOff,
                     float outerCutOff, float constant, float linear,
                     float quadratic)
        : PointLight{ambient,  diffuse, specular, position,
                     constant, linear,  quadratic},
          direction_{direction}, cut_off_{cutOff}, outer_cut_off_{outerCutOff} {
}

void SpotLight::apply(Shader* shader, const std::string& fieldName) const {
    PointLight::apply(shader, fieldName);
    shader->set(fieldName + ".direction", direction_);
    shader->set(fieldName + ".cutOff", cut_off_);
    shader->set(fieldName + ".outerCutOff", outer_cut_off_);
}
std::string SpotLight::getShaderFieldName() const {
    return "spotLights";
}

SpotLight::Builder::Builder(const glm::vec3& position,
                            const glm::vec3& direction)
        : PointLight::Builder(position), b_direction_{direction} {
}

SpotLight::Builder& SpotLight::Builder::set_cut_off(float cutOff) {
    b_cut_off_ = cutOff;
    return *this;
}

SpotLight::Builder& SpotLight::Builder::set_outer_cut_off(float outerCutOff) {
    b_outer_cut_off_ = outerCutOff;
    return *this;
}

SpotLight* SpotLight::Builder::build() {
    return new SpotLight{b_ambient_,       b_diffuse_,   b_specular_,
                         b_position_,      b_direction_, b_cut_off_,
                         b_outer_cut_off_, b_constant_,  b_linear_,
                         b_quadratic_};
}
} // namespace rg