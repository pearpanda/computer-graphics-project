#include <rg/renderer/light/Light.hpp>

namespace rg {

Light::Light(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular)
        : ambient_{ambient}, diffuse_{diffuse}, specular_{specular} {
}

void Light::apply(Shader* shader, const std::string& fieldName) const {
    shader->set(fieldName + ".ambient", ambient_);
    shader->set(fieldName + ".specular", specular_);
    shader->set(fieldName + ".diffuse", diffuse_);
}

Light::Builder& Light::Builder::set_ambient(const glm::vec3& ambient) {
    b_ambient_ = ambient;
    return *this;
}

Light::Builder& Light::Builder::set_diffuse(const glm::vec3& diffuse) {
    b_diffuse_ = diffuse;
    return *this;
}

Light::Builder& Light::Builder::set_specular(const glm::vec3& specular) {
    b_specular_ = specular;
    return *this;
}
} // namespace rg