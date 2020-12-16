#include <rg/renderer/light/DirectionalLight.hpp>

namespace rg {

void DirectionalLight::apply(const Shader& shader,
                             const std::string& fieldName) {
    Light::apply(shader, fieldName);
    shader.set(fieldName + ".direction", direction);
}
} // namespace rg