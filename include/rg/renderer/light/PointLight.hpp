#ifndef RG_POINTLIGHT_HPP
#define RG_POINTLIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class PointLight : public Light {
public:
    void apply(Shader* shader, const std::string& fieldName) const override;
    [[nodiscard]] inline std::string getShaderFieldName() const override;

    ~PointLight() override = default;

    class Builder;

protected:
    PointLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
               glm::vec3 position, float constant, float linear,
               float quadratic);

private:
    glm::vec3 position_;

    float constant_;
    float linear_;
    float quadratic_;
};

class PointLight::Builder : public Light::Builder {

public:
    // position is a required parameter - no sensible default
    explicit Builder(const glm::vec3& position);

    // these 3 can be one method, but you'll probably never call set_constant
    PointLight::Builder& set_constant(float constant);
    PointLight::Builder& set_linear(float linear);
    PointLight::Builder& set_quadratic(float quadratic);

    PointLight* build() override;

protected:
    const glm::vec3 position_;

    float constant_ = 1.0;
    float linear_ = 0.22;
    float quadratic_ = 0.2;
};

} // namespace rg
#endif // RG_POINTLIGHT_HPP
