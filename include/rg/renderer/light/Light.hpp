#ifndef RG_LIGHT_HPP
#define RG_LIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class Light {
public:
    virtual void apply(Shader* shader, const std::string& fieldName) const;
    [[nodiscard]] virtual std::string getShaderFieldName() const = 0;

    virtual ~Light() = default;

    class Builder;

protected:
    Light(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular);

private:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
};

class Light::Builder {
public:
    // these are manipulating private fields, so they are not virtual
    Light::Builder& set_ambient(const glm::vec3& ambient);
    Light::Builder& set_diffuse(const glm::vec3& diffuse);
    Light::Builder& set_specular(const glm::vec3& specular);

    virtual Light* build() = 0;

protected:
    glm::vec3 ambient_ = glm::vec3{0.1f, 0.1f, 0.1f};
    glm::vec3 diffuse_ = glm::vec3{0.3f, 0.3f, 0.3f};
    glm::vec3 specular_ = glm::vec3{1.0f, 1.0f, 1.0f};
};
} // namespace rg
#endif // RG_LIGHT_HPP
