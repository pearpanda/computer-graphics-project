#include <rg/renderer/light/Light.hpp>

namespace rg {

void Light::apply(const Shader& shader, const std::string& fieldName) {
    shader.set(fieldName + ".ambient_", ambient_);
    shader.set(fieldName + ".specular", specular_);
    shader.set(fieldName + ".diffuse", diffuse_);
}

} // namespace rg