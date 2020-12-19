#include <rg/renderer/light/PointLight.hpp>

namespace rg {

PointLight::PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                       glm::vec3 position, float constant, float linear,
                       float quadratic)
        : Light{ambient, diffuse, specular}, position_{position},
          constant_{constant}, linear_{linear}, quadratic_{quadratic} {
}

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

PointLight::Builder::Builder(const glm::vec3& position)
        : b_position_{position} {
}

PointLight::Builder& PointLight::Builder::set_constant(float constant) {
    b_constant_ = constant;
    return *this;
}
PointLight::Builder& PointLight::Builder::set_linear(float linear) {
    b_linear_ = linear;
    return *this;
}
PointLight::Builder& PointLight::Builder::set_quadratic(float quadratic) {
    b_quadratic_ = quadratic;
    return *this;
}

PointLight* PointLight::Builder::build() {
    return new PointLight{b_ambient_,  b_diffuse_, b_specular_, b_position_,
                          b_constant_, b_linear_,  b_quadratic_};
}
} // namespace rg
