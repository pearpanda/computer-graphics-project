#ifndef RG_DIRECTIONALLIGHT_HPP
#define RG_DIRECTIONALLIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class DirectionalLight : public Light {
public:
    void apply(Shader* shader, const std::string& fieldName) const override;
    [[nodiscard]] inline std::string getShaderFieldName() const override;

    ~DirectionalLight() override = default;

    class Builder;

protected:
    DirectionalLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                     glm::vec3 direction);

private:
    glm::vec3 direction_;
};

class DirectionalLight::Builder : public Light::Builder {
public:
    explicit Builder(const glm::vec3& direction);

    DirectionalLight* build() override;

protected:
    glm::vec3 b_direction_;
};

} // namespace rg
#endif // RG_DIRECTIONALLIGHT_HPP
