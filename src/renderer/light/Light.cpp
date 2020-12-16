#include <rg/renderer/light/Light.hpp>

namespace rg {

void Light::apply(const Shader& shader, const std::string& fieldName) {
    shader.set(fieldName + ".ambient", ambient);
    shader.set(fieldName + ".specular", specular);
    shader.set(fieldName + ".diffuse", diffuse);
}

} // namespace rg