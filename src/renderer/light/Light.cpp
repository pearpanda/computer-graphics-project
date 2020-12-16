#include <rg/renderer/light/Light.hpp>

namespace rg {

void Light::apply(Shader* shader, const std::string& fieldName) const {
    shader->set(fieldName + ".ambient", ambient_);
    shader->set(fieldName + ".specular", specular_);
    shader->set(fieldName + ".diffuse", diffuse_);
}

} // namespace rg