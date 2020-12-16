#include <rg/renderer/light/DirectionalLight.hpp>

namespace rg {

void DirectionalLight::apply(Shader* shader,
                             const std::string& fieldName) const {
    Light::apply(shader, fieldName);
    shader->set(fieldName + ".direction_", direction_);
}
std::string DirectionalLight::getShaderFieldName() const {
    return "dirLights";
}
} // namespace rg